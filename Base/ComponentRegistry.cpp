#include "ComponentRegistry.h"

namespace
{
	static GameBase::ComponentIndex indexCounter_;  // 型Idを増やすやつ
	static std::vector<std::weak_ptr<void>> pComponentPools_;
	static std::vector<std::string_view> componentTypeNames_;
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

std::vector<std::string_view>& GameBase::ComponentRegistry::ComponentTypeNames()
{
	return componentTypeNames_;
}

GameBase::ComponentIndex GameBase::ComponentRegistry::GetComponentIndex(const std::string_view _name)
{
	auto itr{ std::find(componentTypeNames_.begin(), componentTypeNames_.end(), _name) };
	GB_ASSERT(itr != componentTypeNames_.end()
		&& "コンポーネント名で検索したが見つからなかった");

	return itr - componentTypeNames_.begin();
}

GameBase::IComponentPool& GameBase::Get(const ComponentIndex _index)
{
	auto sp{ pComponentPools_[_index].lock() };
	auto spIComponentPool{ std::static_pointer_cast<IComponentPool>(sp) };
	return *spIComponentPool.get();
}
