#include "ImageRenderer.h"
#include <Component/Image.h>
#include <Component/Transform.h>
#include "../GameTime.h"
#include "../TransformCalculator.h"
#include "../MaterialRegistry.h"
#include "../MeshRegistry.h"
#include "../Renderer.h"


GameBase::System::ImageRenderer::ImageRenderer()
{}

GameBase::System::ImageRenderer::~ImageRenderer()
{}

void GameBase::System::ImageRenderer::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>*_registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<TransformCalculator>())
		->Add(SystemRegistry::GetSystemIndex<MeshRegistry>())
		->Add(SystemRegistry::GetSystemIndex<MaterialRegistry>())
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		;
}

void GameBase::System::ImageRenderer::Initialize()
{}

void GameBase::System::ImageRenderer::Update(EntityRegistry&)
{
	using Component::Transform;
	using Component::Image;

	if (!Get<GameTime>().IsFrameDue())
	{
		return;  // 描画タイミング以外は無視
	}
}

void GameBase::System::ImageRenderer::Release()
{}
