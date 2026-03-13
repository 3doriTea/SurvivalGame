#include "pch.h"
#include "Input.h"
#include "GameTime.h"


GameBase::System::Input::Input()
{
}

GameBase::System::Input::~Input()
{
}

void GameBase::System::Input::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		;
}

void GameBase::System::Input::Initialize()
{
}

void GameBase::System::Input::Update()
{
	if (Get<GameTime>().IsFrameDue() == false)
	{
		return;
	}


}

void GameBase::System::Input::Release()
{
}

bool GameBase::System::Input::IsPress(const KeyCode _keyCode)
{
	return false;
}

bool GameBase::System::Input::IsPressDown(const KeyCode _keyCode)
{
	return false;
}

bool GameBase::System::Input::IsPressUp(const KeyCode _keyCode)
{
	return false;
}
