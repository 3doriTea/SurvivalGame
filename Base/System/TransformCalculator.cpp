#include "TransformCalculator.h"


GameBase::System::TransformCalculator::TransformCalculator() :
	checked_{},
	calculationQueue_{}
{}

GameBase::System::TransformCalculator::~TransformCalculator()
{}

void GameBase::System::TransformCalculator::Initialize()
{}

void GameBase::System::TransformCalculator::Update(EntityRegistry& _registry)
{
	using namespace DirectX;

	View<Component::Transform> view{ _registry.GetView<Component::Transform>() };

	checked_.clear();
	calculationQueue_.clear();

	view.ForEach([&view, this](const Entity entity, Component::Transform& transform)
	{
		Calculate(view, entity, transform);
	});

	for (const Entity entity : calculationQueue_)
	{
		auto [t]{ view.At(entity) };
		t.localTranslateMatrix = XMMatrixTranslation(t.position.x, t.position.y, t.position.z);
		t.localRotationMatrix = XMMatrixRotationQuaternion(t.rotation);
		t.localScaleMatrix = XMMatrixScaling(t.scale.x, t.scale.y, t.scale.z);

		if (t.parent)
		{
			auto [parentT]{ view.At(entity) };
			t.worldMatrix =
				t.localTranslateMatrix *
				t.localRotationMatrix *
				t.localScaleMatrix *
				parentT.worldMatrix;
		}
		else
		{
			t.worldMatrix =
				t.localTranslateMatrix *
				t.localRotationMatrix *
				t.localScaleMatrix;
		}
	}
}

void GameBase::System::TransformCalculator::Release()
{}

void GameBase::System::TransformCalculator::Calculate(
	View<Component::Transform>& _view,
	const Entity _entity,
	Component::Transform & _transform)
{
	if (checked_.count(_entity) > 0)
	{
		return;
	}

	if (_transform.parent != INVALID_ENTITY)
	{
		auto [parentTransform]{ _view.At(_transform.parent) };
		Calculate(_view, _transform.parent, parentTransform);
	}

	checked_.insert(_entity);
	calculationQueue_.push_back(_entity);
}
