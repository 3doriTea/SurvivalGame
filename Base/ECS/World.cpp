#include "World.h"
#include "../SystemBase.h"
#include "../DependencyResolver.h"
#include "../Standalone/Utility/ReferencePrinter.h"
#include "../Structure/AssetLoader/YamlLoader.h"
#include "../ComponentBase.h"
#include "../Structure/Schema/YamlSchema.h"



GameBase::World::World(const EntityVersion _version) :
	VERSION_{ _version },
	systemOrderIndices_{}
{}

bool GameBase::World::TryLoadScene(
	const fs::path& _sceneFile,
	const bool _initializeSystem)
{
	pRegistry_ = std::make_unique<EntityRegistry>(VERSION_);

	// TODO: ファイルから読み込む

	const std::string SCENE_NAME{ _sceneFile.stem().string() };

	Debugger::LogBegin("LoadScene:" + SCENE_NAME);

	if (_initializeSystem)
	{
		if (bool succeed{ TryReloadSystems() };
			succeed == false)
		{
			return false;
		}
	}

	Schema::YamlSchema yaml{};

#pragma region シーンのYAMLファイル読み込み
	YamlLoader loader{};
	loader.OnNode([&yaml](
		const std::string& _tag,
		const std::string& _sceneName,
		const YAML::Node& _node)
	{
		using namespace Schema;

		Debugger::LogF("tag:{}, scene:{}", _tag, _sceneName);

		try
		{
			if (_tag == "GameBase::Scene")
			{
				GameScene gameScene{};
				gameScene.name = _sceneName;

				const YAML::Node& gameObjects{ _node["Scene"]["gameObjects"] };
				for (const YAML::Node& gameObject : gameObjects)
				{
					gameScene.gameObjects.push_back(gameObject["gameObject"]["fileId"].as<FileId>());
				}

				yaml.gameScenes.push_back(gameScene);
			}
			else if (_tag == "GameBase::GameObject")
			{
				GameObject gameObject{};
				gameObject.self = _node["Id"].as<FileId>();
				gameObject.name = _node["GameObject"]["name"].as<std::string>();
				const YAML::Node& components{ _node["GameObject"]["components"] };
				for (const YAML::Node& component : components)
				{
					gameObject.gameComponents.push_back(component["component"]["fileId"].as<FileId>());
				}

				yaml.gameObjects.push_back(gameObject);
			}
			else if (std::string componentTag{ "GameBase::Component::" };
				StringUtil::StartWith(_tag, componentTag))
			{
				std::string_view tag{ _tag };
				std::string_view componentName{ tag.substr(componentTag.size()) };

				GameComponent gameComponent{};
				gameComponent.self = _node["Id"].as<FileId>();
				gameComponent.tag = _tag;
				gameComponent.node = _node[componentName];

				yaml.gameComponents.push_back(gameComponent);
			}
			else
			{
				GB_ASSERT(false && "知らないタグ", _tag);
			}
		}
		catch (const YAML::Exception& ex)
		{
			GB_ASSERT(false && "読み取り例外", ex.what());
		}
	});

	bool succeed{ loader.TryLoad(_sceneFile) };
	GB_ASSERT(succeed);

	Debugger::LogWriteOutFile("./Logs/", "__YAMLLLLLLL");
#pragma endregion

#pragma region EntityComponent配置していく
	// コンポーネントの Id to ポインタ を作る
	std::map<Schema::FileId, Schema::GameComponent*> fileIdToComponent{};

	for (auto& component : yaml.gameComponents)
	{
		// FileIdの連想配列に追加、あればエラー
		GB_ASSERT(fileIdToComponent.count(component.self) == 0
			&& "コンポーネントのFileIdに重複がある",
			std::format("component tag:{}, >>id:{}<<, there",
				component.tag,
				component.self,
				fileIdToComponent.at(component.self)->tag));

		fileIdToComponent.emplace(component.self, &component);
	}

	// エンティティを作っていく
	for (auto& gameObject : yaml.gameObjects)
	{
		Entity entity{ pRegistry_.get()->CreateEntity() };

		for (const Schema::FileId componentId : gameObject.gameComponents)
		{
			GB_ASSERT(fileIdToComponent.count(componentId) > 0
				&& "ゲームオブジェクトが存在しないコンポーネントFileIdを参照している",
				std::format("object:{}, unknown comp fileId:{}", gameObject.name, componentId));

			Schema::GameComponent& component{ *fileIdToComponent.at(componentId) };

			ComponentIndex index{ ComponentRegistry::GetComponentIndex("struct " + component.tag) };
			IComponentBase& registryComponent{ pRegistry_.get()->AddComponent(entity, index) };
			try
			{
				registryComponent.OnLoad(component.node);
			}
			catch (const YAML::Exception& ex)
			{
				GB_ASSERT(false && "コンポーネントの読み取りに失敗",
					std::format("component:{} why->{}", component.tag, ex.what()));
				return false;
			}
		}
	}
#pragma endregion

	return true;
}

bool GameBase::World::Update()
{
	auto pSystems{ SystemRegistry::PSystems() };

	for (const int index : systemOrderIndices_)
	{
		if (auto pSystem{ pSystems[index].lock() })
		{
			pSystem->Update(*pRegistry_.get());
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool GameBase::World::Release()
{
	auto pSystems{ SystemRegistry::PSystems() };

	// NOTE: 初期化とは逆の順番に解放していく
	for (auto itr = systemOrderIndices_.rbegin();
		itr != systemOrderIndices_.rend();
		itr++)
	{
		if (auto pSystem{ pSystems.at(*itr).lock() })
		{
			pSystem->Release();
			pSystem.reset();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool GameBase::World::TryReloadSystems()
{
	Debugger::LogBegin("TryReloadSystems");

#pragma region コンストラクタ呼び出し後に登録していく
	Debugger::LogBegin("ConstructorFunc");
	while (SystemRegistry::RegisterQueue().size() > 0)
	{
		// TODO: ここにシーン名入れてシステム除外処理を追加する
		SystemRegistry::RegisterQueue().front()();
		SystemRegistry::RegisterQueue().pop();
	}

	while (ComponentRegistry::RegisterQueue().size() > 0)
	{
		ComponentRegistry::RegisterQueue().front()();
		ComponentRegistry::RegisterQueue().pop();
	}
	Debugger::LogEnd();
#pragma endregion

#pragma region システム間の依存関係からソート
	using SystemIndex = size_t;

	DependencyResolver resolver{};

	std::stringstream ss{};
	SystemIndex idx{};
	for (auto p : SystemRegistry::PSystems())
	{
		if (auto pSystem{ p.lock() })
		{
			FluentVector<SystemIndex> registry{};
			pSystem->OnRegisterDependencies(
				static_cast<FluentVectorAddOnly<SystemIndex>*>(
					&registry));

			ss << "system:" << std::setw(2) << idx << "->";

			std::vector<int> dependency(registry.size());
			size_t i{};
			for (SystemIndex systemIndex : registry)
			{
				dependency.at(i) = static_cast<int>(systemIndex);
				ss << std::setw(2) << i << ",";
				i++;
			}

			resolver.AddDependency(dependency);
		}
		else
		{
			return false;
		}
		ss << std::endl;
		idx++;
	}

	resolver.Resolve();

	if (resolver.Deadlocked())
	{
		Debugger::LogBegin("DependencyResolve");

		Debugger::LogF("system index 依存関係エラー");
		resolver.ForEachDeadlocked([](int _systemIndex)
			{
				Debugger::LogF("未解決:{:2}", _systemIndex);
			});
		Debugger::LogEnd();
		return false;
	}

	// 依存関係エラーなし
	systemOrderIndices_.resize(SystemRegistry::PSystems().size());
	size_t i = 0;
	Debugger::LogBegin("SystemResult");
	resolver.ForEachResult([this, &i](int _systemIndex)
		{
			systemOrderIndices_.at(i) = _systemIndex;
			Debugger::LogF("ソート済み :{:2}", _systemIndex);
			i++;
		});
	std::reverse(systemOrderIndices_.begin(), systemOrderIndices_.end());
	Debugger::LogEnd();

	Debugger::LogBegin("SystemRequired");
	Debugger::LogF("{}", ss.str());
	Debugger::LogEnd();

	std::vector<std::pair<size_t, std::vector<size_t>>> mapping{};

	for (const int index : systemOrderIndices_)
	{
		if (auto pSystem{ SystemRegistry::PSystems().at(index).lock() })
		{
			FluentVector<SystemIndex> registry{};
			pSystem->OnRegisterDependencies(
				static_cast<FluentVectorAddOnly<SystemIndex>*>(
					&registry));

			mapping.push_back({ index, registry.Unwarp() });
		}
		else
		{
			return false;
		}
	}

	std::string output{ ReferencePrinter(mapping) };
	Debugger::LogF("\n{}", output);

#pragma endregion

#pragma region 全システムの初期化処理
	auto pSystems{ SystemRegistry::PSystems() };

	for (const int index : systemOrderIndices_)
	{
		if (auto pSystem{ pSystems.at(index).lock() })
		{
			pSystem->Initialize();
		}
		else
		{
			return false;
		}
	}
#pragma endregion

	Debugger::LogWriteOutFile("./Logs/", "dumpSystemList");

	Debugger::LogEnd();

	return true;
}
