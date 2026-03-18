#include "SystemBase.h"


namespace
{
}

GameBase::SystemIndex& GameBase::SystemRegistry::IndexCounter()
{
	static GameBase::SystemIndex indexCounter_;  // 型Idを増やすやつ
	return indexCounter_;
}

std::vector<std::weak_ptr<GameBase::ISystemBase>>& GameBase::SystemRegistry::PSystems()
{
	static std::vector<std::weak_ptr<GameBase::ISystemBase>> pSystems_;
	return pSystems_;
}

std::vector<std::weak_ptr<void>>& GameBase::SystemRegistry::PInterfaces()
{
	static std::vector<std::weak_ptr<void>> pInterfaces_;
	return pInterfaces_;
}

std::queue<std::function<void()>>& GameBase::SystemRegistry::RegisterQueue()
{
	static std::queue<std::function<void()>> registerQueue_;
	return registerQueue_;
}

std::queue<std::function<void()>>& GameBase::SystemRegistry::DestructionQueue()
{
	static std::queue<std::function<void()>> destructionQueue_;
	return destructionQueue_;
}
