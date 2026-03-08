#include "ComponentBase.h"

namespace
{
	static GameBase::ComponentIndex indexCounter_;  // 型Idを増やすやつ
	static std::vector<std::weak_ptr<void>> pComponentPools_;
	static std::queue<std::function<void()>> registerQueue_;
}

GameBase::ComponentIndex& GameBase::ComponentRegistry::IndexCounter()
{
	return indexCounter_;
}

std::vector<std::weak_ptr<void>>& GameBase::ComponentRegistry::PComponentPools()
{
	return pComponentPools_;
}

std::queue<std::function<void()>>& GameBase::ComponentRegistry::RegisterQueue()
{
	return registerQueue_;
}
