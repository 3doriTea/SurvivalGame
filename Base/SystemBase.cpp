#include "SystemBase.h"

namespace
{
	static GameBase::SystemIndex indexCounter_;  // 型Idを増やすやつ
	static std::vector<std::weak_ptr<GameBase::ISystemBase>> pSystems_;
	static std::vector<std::weak_ptr<void>> pInterfaces_;
	static std::queue<std::function<void()>> registerQueue_;
}

GameBase::SystemIndex& GameBase::SystemRegistry::IndexCounter()
{
	return indexCounter_;
}

std::vector<std::weak_ptr<GameBase::ISystemBase>>& GameBase::SystemRegistry::PSystems()
{
	return pSystems_;
}

std::vector<std::weak_ptr<void>>& GameBase::SystemRegistry::PInterfaces()
{
	return pInterfaces_;
}

std::queue<std::function<void()>>& GameBase::SystemRegistry::RegisterQueue()
{
	return registerQueue_;
}
