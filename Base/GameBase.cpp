#include "GameBase.h"


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

	// TODO: デフォルトシーン読み込み
	MakeScene();

	Debugger::LogEnd();
	return true;
}

bool GameBase::Game::Update()
{
	if (!pWorld_)
	{
		return false;
	}
	return pWorld_.get()->Update();
}

bool GameBase::Game::End()
{
	if (!pWorld_)
	{
		return false;
	}
	return pWorld_.get()->Release();
}

void GameBase::Game::MakeScene()
{
	versionCounter_++;  // バージョンを上げる
	pWorld_ = std::make_unique<World>(versionCounter_);
}

void GameBase::Game::MoveScene(const fs::path& _sceneFile)
{
	if (!pWorld_)
	{
		MakeScene();  // ワールドがないなら作る
	}

	bool succeed{ pWorld_->TryLoadScene(_sceneFile) };
	if (succeed == false)
	{
		Debugger::Log("Faild world->TryLoadScene");
	}
}
