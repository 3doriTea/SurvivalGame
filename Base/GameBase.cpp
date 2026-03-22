#include "GameBase.h"
#include "GameEvent.h"

#include "GameEvent/GameExit.h"
#include "GameEvent/ChangeScene.h"
#include "GameEvent/ChangeRunMode.h"

#include "SystemBase.h"
#include "ComponentBase.h"


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

GameBase::Game::Game(const RunMode _runMode) :
	versionCounter_{},
	runMode_{ _runMode }
{
}

bool GameBase::Game::Start()
{
	Debugger::LogBegin("GameBase::Game::Start");

	//// TODO: デフォルトシーン読み込み
	MoveScene(fs::current_path() / "Assets/Default/Boot.scene.yaml");
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

	GameEvent::TryErase<GameEvent::ChangeRunMode>([this](GameEvent::ChangeRunMode& _event)
	{
		runMode_ = _event.runMode;
	});

	// シーン遷移イベントの処理
	GameEvent::TryErase<GameEvent::ChangeScene>([this](GameEvent::ChangeScene& _event)
	{
		MoveScene(_event.nextSceneFile);
	});

	return true;
}

bool GameBase::Game::End()
{
	bool succeed{  };
	if (pWorld_)
	{
		succeed = pWorld_.get()->Release();
	}
	else
	{
		succeed = false;
	}

	SystemRegistry::PInterfaces().clear();
	SystemRegistry::PSystems().clear();
	ComponentRegistry::PComponentPools().clear();

	// 静的領域にインスタンスしたシステムたちをmain関数が終わる前に消す
	// NOTE: プログラム終了に任せるとエラーがでる
	while (!SystemRegistry::DestructionQueue().empty())
	{
		SystemRegistry::DestructionQueue().front()();
		SystemRegistry::DestructionQueue().pop();
	}

	return succeed;
}

void GameBase::Game::MakeScene(const RunInfo& _runInfo, const bool _reloadSystems)
{
	versionCounter_++;  // バージョンを上げる
	pWorld_ = std::make_unique<World>(versionCounter_, _runInfo);
	if (_reloadSystems)
	{
		pWorld_.get()->TryReloadSystems();
	}
}

void GameBase::Game::MoveScene(const fs::path& _sceneFile)
{
	if (!pWorld_)
	{
		std::string fileName{ _sceneFile.filename().string() };
		std::string sceneName{ fileName.substr(0, fileName.find('.')) };

		MakeScene(RunInfo{ runMode_, sceneName }, true);  // ワールドがないなら作る
	}

	bool succeed{ pWorld_->TryLoadScene(_sceneFile) };
	if (succeed == false)
	{
		Debugger::Log("Faild world->TryLoadScene");
	}
}
