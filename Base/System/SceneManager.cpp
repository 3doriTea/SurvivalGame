#include "SceneManager.h"
#include "Assets.h"


GameBase::System::SceneManager::SceneManager()
{}

GameBase::System::SceneManager::~SceneManager()
{}

void GameBase::System::SceneManager::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>*_registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<Assets>())
	;
}

void GameBase::System::SceneManager::Initialize()
{
	if (currentSceneFile_.empty())
	{
		
	}
}

void GameBase::System::SceneManager::Update()
{}

void GameBase::System::SceneManager::Release()
{}
