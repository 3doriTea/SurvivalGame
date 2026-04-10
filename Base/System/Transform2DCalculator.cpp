#include "Transform2DCalculator.h"
#include "ViewportSwitcher.h"

GameBase::System::Transform2DCalculator::Transform2DCalculator() :
	checked_{},
	calculationQueue_{}
{}

GameBase::System::Transform2DCalculator::~Transform2DCalculator()
{}

void GameBase::System::Transform2DCalculator::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<ViewportSwitcher>())
	;
}

void GameBase::System::Transform2DCalculator::Initialize()
{}

void GameBase::System::Transform2DCalculator::Update(EntityRegistry& _registry)
{
	using namespace DirectX;

	View<Component::Transform2D> view{ _registry.GetView<Component::Transform2D>() };

	checked_.clear();
	calculationQueue_.clear();

	view.ForEach([&view, this](const Entity entity, Component::Transform2D& transform)
		{
			Calculate(view, entity, transform);
		});

	const Vec2 VIEWPORT_SIZE{ Get<ViewportSwitcher>().GetSizeAt(ViewportMode::Gamer) };
	GB_ASSERT(VIEWPORT_SIZE.x > 0.0f && VIEWPORT_SIZE.y > 0.0f
		&& "ビューポートサイズが0でゼロ除算発生");

	for (const Entity entity : calculationQueue_)
	{
		auto [t] { view.At(entity) };

		// MEMO: 単位行列
		//     : 拡縮して
		//     : それに軸をつけて左上基準に動かして
		//     : 回転させて
		//     : 平行移動
		//     : 基準軸で平行移動
		//     : ビューポートサイズに縮小
		/*t.localMatrix = XMMatrixIdentity()
			* XMMatrixScaling(t.scale.x, t.scale.y, 1.0)
			* XMMatrixTranslation(
				t.pivotPoint.x * -(t.scale.x * 2.0f) + t.scale.x,
				t.pivotPoint.y * +(t.scale.y * 2.0f) - t.scale.y,
				0.0f)
			* XMMatrixRotationZ(t.angle)
			* XMMatrixTranslation(
				+t.position.x - VIEWPORT_SIZE.x / 2.0f,
				-t.position.y + VIEWPORT_SIZE.y / 2.0f,
				0.0f)
			* XMMatrixTranslation(
				t.anchorPoint.x * +(VIEWPORT_SIZE.x),
				t.anchorPoint.y * -(VIEWPORT_SIZE.y),
				0.0f)
			* XMMatrixScaling(2.0f / VIEWPORT_SIZE.x, 2.0f / VIEWPORT_SIZE.y, 1.0f)
			;*/

		if (t.parent)
		{
			// 親がいるなら親の情報を考慮してかける
			auto [parentT] { view.At(entity) };

			float offsetX{ +t.position.x + (t.anchorPoint.x - 0.5f) * parentT.scale.x };
			float offsetY{ -t.position.y + (0.5f - t.anchorPoint.y) * parentT.scale.y };

			t.localMatrix = XMMatrixIdentity()
				* XMMatrixScaling(t.scale.x, t.scale.y, 1.0f)
				* XMMatrixTranslation(
					(1.0f - 2.0f * t.pivotPoint.x) * t.scale.x,
					(2.0f * t.pivotPoint.y - 1.0f) * t.scale.y,
					0.0f)
				* XMMatrixRotationZ(t.angle)
				* XMMatrixTranslation(offsetX, offsetY, 0.0f);

			t.toChildMatrix = XMMatrixIdentity()
				* XMMatrixRotationZ(t.angle)
				* XMMatrixTranslation(offsetX, offsetY, 0.0f)
				* parentT.toChildMatrix;

			t.worldMatrix = t.localMatrix
				* parentT.toChildMatrix;
		}
		else
		{
			float offsetX{ +t.position.x + (t.anchorPoint.x - 0.5f) * VIEWPORT_SIZE.x };
			float offsetY{ -t.position.y + (0.5f - t.anchorPoint.y) * VIEWPORT_SIZE.y };

			t.localMatrix = XMMatrixIdentity()
				* XMMatrixScaling(t.scale.x, t.scale.y, 1.0f)
				* XMMatrixTranslation(
					(1.0f - 2.0f * t.pivotPoint.x) * t.scale.x,
					(2.0f * t.pivotPoint.y - 1.0f) * t.scale.y,
					0.0f)
				* XMMatrixRotationZ(t.angle)
				* XMMatrixTranslation(offsetX, offsetY, 0.0f);

			t.toChildMatrix = XMMatrixIdentity()
				* XMMatrixRotationZ(t.angle)
				* XMMatrixTranslation(offsetX, offsetY, 0.0f);

			t.worldMatrix = t.localMatrix;
		}
	}
}

void GameBase::System::Transform2DCalculator::Release()
{}

void GameBase::System::Transform2DCalculator::Calculate(View<Component::Transform2D>&_view, const Entity _entity, Component::Transform2D & _transform)
{
	if (checked_.count(_entity) > 0)
	{
		return;
	}

	if (_transform.parent != INVALID_ENTITY)
	{
		auto [parentTransform] { _view.At(_transform.parent) };
		Calculate(_view, _transform.parent, parentTransform);
	}

	checked_.insert(_entity);
	calculationQueue_.push_back(_entity);
}
