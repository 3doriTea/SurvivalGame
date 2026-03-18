#include "Renderer.h"
#include "GameTime.h"
#include "Presenter.h"
#include "Graphic/MainCamera.h"


GameBase::System::Renderer::Renderer() :
	beginEvent_{ Event<EntityRegistry&>::Create() },
	endEvent_{ Event<EntityRegistry&>::Create() },
	renderEvent_{ Event<EntityRegistry&>::Create() }
{
}

GameBase::System::Renderer::~Renderer()
{
}

void GameBase::System::Renderer::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		->Add(SystemRegistry::GetSystemIndex<Presenter>())
		->Add(SystemRegistry::GetSystemIndex<MainCamera>())
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
			return _left.sortKey.value > _right.sortKey.value;
		});

	beginEvent_.get()->Invoke(_registry);

	Get<Presenter>().BeginDraw();

	// レンダーキューの処理をする
	// 各ウィンドウ分、カメラ別で描画していく

	for (auto& item : renderQueue_)
	{
		//item.pMaterial->
	}

	renderEvent_.get()->Invoke(_registry);

	Get<Presenter>().EndDraw();

	endEvent_.get()->Invoke(_registry);

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

void GameBase::System::Renderer::OnEnd(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(endEvent_);
}

void GameBase::System::Renderer::Enqueue(RenderItem&& _renderItem)
{
	renderQueue_.emplace_back(std::move(_renderItem));
}
