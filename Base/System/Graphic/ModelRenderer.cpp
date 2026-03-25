#include "ModelRenderer.h"
#include "../ModelRegistry.h"
#include "../MeshRegistry.h"
#include "../TransformCalculator.h"
#include "../MaterialRegistry.h"
#include "../GameTime.h"
#include <Component/Transform.h>
#include <Component/ModelRenderable.h>
#include "../Renderer.h"


GameBase::System::ModelRenderer::ModelRenderer()
{}

GameBase::System::ModelRenderer::~ModelRenderer()
{}

void GameBase::System::ModelRenderer::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<TransformCalculator>())
		->Add(SystemRegistry::GetSystemIndex<ModelRegistry>())
		->Add(SystemRegistry::GetSystemIndex<MeshRegistry>())
		->Add(SystemRegistry::GetSystemIndex<MaterialRegistry>())
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
	;
}

void GameBase::System::ModelRenderer::Initialize()
{
}

void GameBase::System::ModelRenderer::Update(EntityRegistry& _registry)
{
	using Component::Transform;
	using Component::ModelRenderable;

	if (!Get<GameTime>().IsFrameDue())
	{
		return;  // 描画タイミング以外は無視
	}

	auto view{ _registry.GetView<Transform, ModelRenderable>() };

	view.ForEach([](const Entity _entity, Transform& _transform, ModelRenderable& _modelRenderable)
		{
			const Model& model{ Get<System::ModelRegistry>().At(_modelRenderable.hModel) };
			for (auto& node : model.nodes)
			{
				Get<Renderer>().Enqueue(RenderItem
					{
						&Get<MeshRegistry>().At(node.hMesh),
						&Get<MaterialRegistry>().At(node.hMaterial),
						_transform.GetWorldMatrix(),
						SortKey::Make(SortKey::Layer_Opaque, 0)
					});
			}
		});
}

void GameBase::System::ModelRenderer::Release()
{}
