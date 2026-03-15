#include "GameBase.h"
#include "GameEvent.h"

#include "GameEvent/GameExit.h"
#include "GameEvent/ChangeScene.h"


#define WINDOWS

#ifdef WINDOWS

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Main();
}

#endif

GameBase::Game::Game() :
	versionCounter_{}
{
}

bool GameBase::Game::Start()
{
	Debugger::LogBegin("GameBase::Game::Start");

	//// TODO: デフォルトシーン読み込み
	MoveScene(fs::current_path() / "Assets/Default/BootScene.yaml");
	//MakeScene(true);

	Debugger::LogEnd();
	return true;
}

bool GameBase::Game::Update()
{
	if (GameEvent::TryErase<GameEvent::GameExit>())
	{
		return false;
	}

	if (!pWorld_)
	{
		return false;
	}

	bool runnable{ pWorld_.get()->Update() };
	if (runnable == false)
	{
		return false;
	}

	// シーン遷移イベントの処理
	GameEvent::TryErase<GameEvent::ChangeScene>([this](GameEvent::ChangeScene& _event)
	{
		MoveScene(_event.nextSceneFile);
	});

	return true;
}

bool GameBase::Game::End()
{
	if (!pWorld_)
	{
		return false;
	}
	return pWorld_.get()->Release();
}

void GameBase::Game::MakeScene(const bool _reloadSystems)
{
	versionCounter_++;  // バージョンを上げる
	pWorld_ = std::make_unique<World>(versionCounter_);
	if (_reloadSystems)
	{
		pWorld_.get()->TryReloadSystems();
	}
}

void GameBase::Game::MoveScene(const fs::path& _sceneFile)
{
	if (!pWorld_)
	{
		MakeScene(true);  // ワールドがないなら作る
	}

	bool succeed{ pWorld_->TryLoadScene(_sceneFile) };
	if (succeed == false)
	{
		Debugger::Log("Faild world->TryLoadScene");
	}
}
