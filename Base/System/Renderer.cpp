#include "Renderer.h"
#include "GameTime.h"
#include "Presenter.h"
#include "Direct3D.h"
#include "Graphic/MainCamera.h"
#include "ViewportSwitcher.h"
#include "TextureRegistry.h"
#include "ShaderCompiler.h"
#include "EnvConstant.h"
#include "../Structure/Renderer/EditorPipeLine.h"
#include "../Structure/Renderer/GamePipeLine.h"


GameBase::System::Renderer::Renderer() :
	beginEvent_{ Event<EntityRegistry&>::Create() },
	renderEvent_{ Event<EntityRegistry&>::Create() },
	releasedTargetEvent_{ Event<EntityRegistry&>::Create() },
	renderLateEvent_{ Event<EntityRegistry&>::Create() },
	endEvent_{ Event<EntityRegistry&>::Create() }
{
}

GameBase::System::Renderer::~Renderer()
{
}

bool GameBase::System::Renderer::OnVerifyShouldSkip(const RunInfo& _runInfo)
{
	switch (_runInfo.mode)
	{
	case RunMode::Editor:
		pPipeLine_ = std::make_unique<EditorPipeLine>();
		break;
	case RunMode::Game:
		pPipeLine_ = std::make_unique<GamePipeLine>();
		break;
	default:
		GB_ASSERT(false && "未対応のワールド実行モード");
		break;
	}

	return false;
}

void GameBase::System::Renderer::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		->Add(SystemRegistry::GetSystemIndex<Presenter>())
		->Add(SystemRegistry::GetSystemIndex<MainCamera>())
		->Add(SystemRegistry::GetSystemIndex<ViewportSwitcher>())
		->Add(SystemRegistry::GetSystemIndex<Direct3D>())
		->Add(SystemRegistry::GetSystemIndex<TextureRegistry>())
		->Add(SystemRegistry::GetSystemIndex<ShaderCompiler>())
		->Add(SystemRegistry::GetSystemIndex<EnvConstant>())
		;
}

void GameBase::System::Renderer::Initialize()
{
}

void GameBase::System::Renderer::Update(EntityRegistry& _registry)
{
	if (!Get<GameTime>().IsFrameDue())
	{
		return;  // フレーム更新タイミング以外は無視
	}

	// 描画キューをソートする
	std::sort(
		renderQueue_.begin(),
		renderQueue_.end(),
		[](const RenderItem& _left, const RenderItem& _right) -> bool
		{
			return _left.sortKey.value < _right.sortKey.value;
		});

	// 描画！
	pPipeLine_.get()->Render(*this, _registry);

	renderQueue_.clear();
}

void GameBase::System::Renderer::Release()
{
}

void GameBase::System::Renderer::OnBegin(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(beginEvent_);
}

void GameBase::System::Renderer::OnRender(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(renderEvent_);
}

void GameBase::System::Renderer::OnReleasedTarget(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(releasedTargetEvent_);
}

void GameBase::System::Renderer::OnRenderLate(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(renderLateEvent_);
}

void GameBase::System::Renderer::OnEnd(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(endEvent_);
}

void GameBase::System::Renderer::Enqueue(RenderItem&& _renderItem)
{
	renderQueue_.emplace_back(std::move(_renderItem));
}
