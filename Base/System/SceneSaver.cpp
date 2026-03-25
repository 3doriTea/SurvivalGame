#include "SceneSaver.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "Input.h"
#include "SchemaLinker.h"
#include "WindowEvent.h"
#include <Structure/Schema/YamlSchema.h>
#include <Component/GameObject.h>
#include <Structure/AssetLoader/YamlSaver.h>


GameBase::System::SceneSaver::SceneSaver() :
	pDangerPtrEntityRegistry_{ nullptr },
	needSaveScene_{ false },
	hasUnsaved_{ false }
{}

GameBase::System::SceneSaver::~SceneSaver()
{}

bool GameBase::System::SceneSaver::OnVerifyShouldSkip(const RunInfo& _info)
{
	// シーンのセーブが必要なのはエディタモードのときのみ
	needSaveScene_ = _info.mode == RunMode::Editor;
	return false;
}

void GameBase::System::SceneSaver::OnRegisterDependencies(
	FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<SceneManager>())
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		->Add(SystemRegistry::GetSystemIndex<SchemaLinker>())
		->Add(SystemRegistry::GetSystemIndex<Input>())
		->Add(SystemRegistry::GetSystemIndex<WindowEvent>())
	;
}

void GameBase::System::SceneSaver::Initialize()
{
	if (needSaveScene_)
	{
		Get<WindowEvent>().RefOnCloseEvent([this](EventSubject<>& _event)
			{
				pWindowDestroyEvnet_ = _event.get()->Connect([this]()
					{
						if (CheckSave())  // キャンセル
						{
							Get<WindowEvent>().CancelCloseWindow();
						}
					});
			});
	}
}

void GameBase::System::SceneSaver::Update(EntityRegistry& _registry)
{
	pDangerPtrEntityRegistry_ = &_registry;
	if (!Get<GameTime>().IsFrameDue())
	{
		return;
	}

	if (Get<Input>().IsPress(KeyCode::LeftControl)
		&& Get<Input>().IsPress(KeyCode::S))
	{
		fs::path currentSceneFile{ Get<SceneManager>().GetCurrentFile() };
		if (currentSceneFile.empty())
		{
			MessageBoxW(
				nullptr,
				"現在のシーンは編集できません。"_w.c_str(),
				"保存できません"_w.c_str(),
				MB_OK);
		}
		else
		{
			TrySaveScene(_registry, currentSceneFile);
		}
	}
}

void GameBase::System::SceneSaver::Release()
{}

bool GameBase::System::SceneSaver::CheckSave()
{
	if (pDangerPtrEntityRegistry_ && hasUnsaved_)
	{
		int result
		{
			MessageBoxW(
				nullptr,
				"シーンは保存されていません。\r\n現在のシーンを保存しますか？"_w.c_str(),
				"作業内容は未保存"_w.c_str(),
				MB_YESNOCANCEL)
		};

		switch (result)
		{
		case IDYES:
			TrySaveScene(
				*pDangerPtrEntityRegistry_,
				Get<SceneManager>().GetCurrentFile());
			hasUnsaved_ = false;
			return false;  // yesならセーブする
		case IDNO:
			hasUnsaved_ = false;
			return false;  // noならそのまま
		case IDCANCEL:
		default:
			return true;  // キャンセル
		}
	}
	return false;
}

bool GameBase::System::SceneSaver::TrySaveScene(EntityRegistry& _registry, const fs::path& _file)
{
	auto view{ _registry.GetView<Component::GameObject>() };

	Schema::YamlSchema yaml{};
	std::vector<Entity> gameObjectsEntity{};

#pragma region シーンを追加
	yaml.gameScenes.push_back({});
	yaml.gameScenes.front().name = Get<SceneManager>().GetCurrentName();

	Schema::FileId fileIdCounter{ 1 };

	std::map<Entity, std::vector<ComponentIndex>> entityToComponentIndex{};

	SchemaLoadBundle loadBundle{};
	const auto& localLoadBundle{ Get<SchemaLinker>().GetSchemaLoadBundle() };
	//loadBundle.assetIdToModel = localLoadBundle.assetIdToModel;
	//loadBundle.modelToAssetId = localLoadBundle.modelToAssetId;
	// ファイルパスを直接指定に変更
	loadBundle.modelToAssetId = Get<SchemaLinker>().GetModelToAssetFile();


#pragma endregion

#pragma region オブジェクトを走査する
	view.ForEach([&entityToComponentIndex, &loadBundle, &fileIdCounter, &yaml, &gameObjectsEntity, _registry]
		(const Entity _entity, Component::GameObject& _gameObject)
		{
			Schema::FileId fileId{ fileIdCounter++ };
			loadBundle.entityToFileId.emplace(_entity, fileId);
			loadBundle.fileIdToEntity.emplace(fileId, _entity);

			entityToComponentIndex.emplace(
				_entity,
				_registry.GetComponentIndices(_entity));

			yaml.gameObjects.push_back(Schema::GameObject
				{
					.entity = _entity,
					.self = fileId,
					.name = _gameObject.name.data(),
					.node = {},
					.gameComponents = {},
				});
			gameObjectsEntity.push_back(_entity);

			// 親階層に接続
			yaml.gameScenes.front().gameObjects.push_back(fileId);
		});
#pragma endregion

#pragma region コンポーネントを走査

	static const std::string COMPONENT_PREFIX{ "struct GameBase::Component::" };

	ComponentIndex gameObjectIndex{ ComponentRegistry::GetComponentIndex<Component::GameObject>()};
	for (auto& [entity, componentIndices] : entityToComponentIndex)
	{
		for (ComponentIndex componentIndex : componentIndices)
		{
			if (gameObjectIndex == componentIndex)
			{
				continue;  // ゲームオブジェクトコンポーネントは無視
			}

			Schema::FileId fileId{ fileIdCounter++ };

			std::string componentName
			{
				ComponentRegistry::ComponentTypeNames()[componentIndex].substr(COMPONENT_PREFIX.size())
			};

			yaml.gameComponents.push_back(Schema::GameComponent
				{
					.self = fileId,
					.index = componentIndex,
					.entity = entity,
					.tag = "!gbc!" + componentName,
					.name = componentName,
					.node = {},
				});

			// 親階層に接続
			auto itr{ std::find(gameObjectsEntity.begin(), gameObjectsEntity.end(), entity) };
			GB_ASSERT(itr != gameObjectsEntity.end()
				&& "ゲームオブジェクト見つからない！");
			int64_t index{ itr - gameObjectsEntity.begin() };
			yaml.gameObjects[index].gameComponents.push_back(fileId);
		}
	}
#pragma endregion

	YamlSaver saver{};

#pragma region シーン書き込み
	saver.WriteNode([&yaml](YAML::Emitter& _emitter)
		{
			_emitter
			<< YAML::BeginMap
				<< YAML::Key << "Scene" << YAML::Value
				<< YAML::BeginMap
					<< YAML::Key << "gameObjects" << YAML::Value
					<< YAML::BeginSeq;
			for (const Schema::FileId& gameObjectFileId : yaml.gameScenes.front().gameObjects)
			{
				_emitter
				<< YAML::BeginMap
				<< YAML::Key << "gameObject" << YAML::Value
					<< YAML::BeginMap
					<< YAML::Key << "fileId" << YAML::Value << gameObjectFileId
					<< YAML::EndMap
				<< YAML::EndMap;
			}
					_emitter << YAML::EndSeq;
				_emitter << YAML::EndMap;
			_emitter << YAML::EndMap;
		},
		"!gb!Scene");
#pragma endregion

#pragma region ゲームオブジェクト書き込み
	for (const auto& gameObject : yaml.gameObjects)
	{
		saver.WriteNode([&yaml, gameObject](YAML::Emitter& _emitter)
			{
				_emitter
				<< YAML::BeginMap
				<< YAML::Key << "Id" << YAML::Value << gameObject.self
				<< YAML::Key << "GameObject" << YAML::Value
					<< YAML::BeginMap
					<< YAML::Key << "name" << YAML::Value << gameObject.name
					<< YAML::Key << "components" << YAML::Value
						<< YAML::BeginSeq;
				for (const Schema::FileId componentFileId : gameObject.gameComponents)
				{
						_emitter
						<< YAML::BeginMap
						<< YAML::Key << "component" << YAML::Value
							<< YAML::BeginMap
							<< YAML::Key << "fileId" << YAML::Value << componentFileId
							<< YAML::EndMap
						<< YAML::EndMap;
				}
						_emitter << YAML::EndSeq
					<< YAML::EndMap
				<< YAML::EndMap;
			},
			"!gb!GameObject");
	}
#pragma endregion

#pragma region コンポーネント書き込み
	for (const auto& component : yaml.gameComponents)
	{
		static const std::string TAG_PREFIX{ "!gbc!" };
		std::string name{ component.tag.substr(TAG_PREFIX.size()) };
		saver.WriteNode([&yaml, &name, &component, &_registry, &loadBundle](YAML::Emitter& _emitter)
			{
				_emitter
				<< YAML::BeginMap
					<< YAML::Key << "Id" << YAML::Value << component.self
					<< YAML::Key << name << YAML::Value
					<< YAML::BeginMap;
						_registry.GetComponent(component.entity, component.index).OnSave(_emitter, loadBundle);
					_emitter << YAML::EndMap
				<< YAML::EndMap;
			},
			component.tag);
	}
#pragma endregion

	return hasUnsaved_ = !saver.TrySave(_file);
}

void GameBase::System::SceneSaver::SetDirty()
{
	hasUnsaved_ = true;
}
