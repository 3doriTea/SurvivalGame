#include "SceneManager.h"
#include "Assets.h"
#include <GameEvent/ChangeScene.h>


GameBase::System::SceneManager::SceneManager() :
	currentSceneFile_{}
{}

GameBase::System::SceneManager::~SceneManager()
{}

void GameBase::System::SceneManager::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<Assets>())
	;
}

void GameBase::System::SceneManager::Initialize()
{
	PreloadNameToFile();
}

void GameBase::System::SceneManager::Update(EntityRegistry&)
{}

void GameBase::System::SceneManager::Release()
{}

std::string GameBase::System::SceneManager::CurrentSceneName()
{
	if (currentSceneFile_.empty())
	{
		return "BootScene";  // 初期シーンを返す
	}
	else
	{
		std::string fileName{ currentSceneFile_.filename().string() };
		return fileName.substr(0, fileName.find('.'));
	}
}

void GameBase::System::SceneManager::PreloadNameToFile()
{
	// シーン名とファイルパスを詰めていく
	nameToFile_.clear();
	Get<Assets>().Ref([this](const std::vector<fs::path>& _file)
		{
			for (const fs::path& file : _file)
			{
				nameToFile_.emplace(CutSceneName(file), file);
			}
		},
		AssetType_YamlScene);
}

std::string GameBase::System::SceneManager::CutSceneName(const fs::path& _sceneYamlFile)
{
	std::string fileName{ _sceneYamlFile.filename().string() };
	return fileName.substr(0, fileName.find('.'));
}

void GameBase::System::SceneManager::LoadSceneFile(const fs::path& _file)
{
	currentSceneFile_ =_file;  // 現在のシーン更新

	// シーンファイルパスからシーン遷移イベント発令
	GameEvent::Emplace<GameEvent::ChangeScene>(_file);
}

void GameBase::System::SceneManager::LoadSceneName(const std::string& _name)
{
	auto itr{ nameToFile_.find(_name) };
	if (itr == nameToFile_.end())
	{
		PreloadNameToFile();
		itr = nameToFile_.find(_name);
	}

	GB_ASSERT(itr != nameToFile_.end()
		&& "シーン名からファイルパスが見つからない");

	LoadSceneFile(itr->second);
}
