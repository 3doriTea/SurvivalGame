#include "ViewportSwitcher.h"
#include "Presenter.h"


GameBase::System::ViewportSwitcher::ViewportSwitcher()
{}

GameBase::System::ViewportSwitcher::~ViewportSwitcher()
{}

void GameBase::System::ViewportSwitcher::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<Presenter>())
		;
}

void GameBase::System::ViewportSwitcher::Initialize()
{
	bool succeed{};

	succeed = Get<Presenter>().TryCreateRenderSurface(&renderSurface_.gamer);
	GB_ASSERT(succeed && "ゲーム画面描画先の作成に失敗");
	succeed = Get<Presenter>().TryCreateRenderSurface(&renderSurface_.editor);
	GB_ASSERT(succeed && "エディタ画面描画先の作成に失敗");
}

void GameBase::System::ViewportSwitcher::Update(EntityRegistry&)
{}

void GameBase::System::ViewportSwitcher::Release()
{}

void GameBase::System::ViewportSwitcher::Switch(const ViewportMode _mode)
{
	Get<Presenter>().SetRenderSurface(&RenderSurfaceAt(_mode));
}

void GameBase::System::ViewportSwitcher::RefAt(const ViewportMode _mode, const std::function<void(const RenderSurface&)>& _callback)
{
	_callback(RenderSurfaceAt(_mode));
}

GameBase::Vec2Int GameBase::System::ViewportSwitcher::GetSizeAt(const ViewportMode _mode)
{
	return RenderSurfaceAt(_mode).size;
}

GameBase::RenderSurface& GameBase::System::ViewportSwitcher::RenderSurfaceAt(const ViewportMode _mode)
{
	switch (_mode)
	{
	case ViewportMode::Gamer:
		return renderSurface_.gamer;
	case ViewportMode::Editor:
		return renderSurface_.editor;
	default:
		GB_ASSERT(false && "他のモードは指定できない");
		break;
	}
}
