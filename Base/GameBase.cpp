#include "GameBase.h"
#include "SystemBase.h"
#include "DependencyResolver.h"
#include "Standalone/Utility/ReferencePrinter.h"

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
	systemOrderIndices_{}
{
}

bool GameBase::Game::Start()
{
	Debugger::LogBegin("GameBase::Game::Start");

#pragma region コンストラクタ呼び出し後に登録していく
	Debugger::LogBegin("ConstructorFunc");
	while (SystemRegistry::RegisterQueue().size() > 0)
	{
		SystemRegistry::RegisterQueue().front()();
		SystemRegistry::RegisterQueue().pop();
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

			ss << "system:" << idx << "->";

			std::vector<int> dependency(registry.size());
			size_t i{};
			for (SystemIndex systemIndex : registry)
			{
				dependency.at(i) = static_cast<int>(systemIndex);
				ss << i << ",";
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
			Debugger::LogF("未解決:{}", _systemIndex);
		});
		Debugger::LogEnd();
		return false;
	}

	// 依存関係エラーなし
	systemOrderIndices_.resize(SystemRegistry::PSystems().size());
	size_t i = 0;
	Debugger::LogBegin("SystemResult");
	/*resolver.ForEachResult([this, &i](int _systemIndex)
	{
		systemOrderIndices_.at(i) = _systemIndex;
		Debugger::LogF("ソート済み :{}", _systemIndex);
		i++;
	});*/
#if 1
	for (auto index : SystemRegistry::PSystems())
	{
		systemOrderIndices_.at(i) = i;
		i++;
	}
#endif
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
	Debugger::LogWriteOutFile("./", "dumpSystemList");

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

	Debugger::LogEnd();
	return true;
}

bool GameBase::Game::Update()
{
	auto pSystems{ SystemRegistry::PSystems() };

	for (const int index : systemOrderIndices_)
	{
		if (auto pSystem{ pSystems[index].lock() })
		{
			pSystem->Update();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool GameBase::Game::End()
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
		}
		else
		{
			return false;
		}
	}

	return true;
}
