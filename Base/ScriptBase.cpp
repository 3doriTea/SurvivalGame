#include "ScriptBase.h"

namespace
{
	static GameBase::ScriptIndex indexCounter_;  // 型Idを増やすやつ
	static std::vector<std::weak_ptr<void>> pSystems_;
	static std::queue<std::function<void()>> registerQueue_;
}

GameBase::ScriptIndex& GameBase::ScriptRegistry::IndexCounter()
{
	return indexCounter_;
}

std::vector<std::weak_ptr<void>>& GameBase::ScriptRegistry::PScripts()
{
	return pSystems_;
}

std::queue<std::function<void()>>& GameBase::ScriptRegistry::RegisterQueue()
{
	return registerQueue_;
}
