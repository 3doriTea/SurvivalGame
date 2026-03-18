#include "ComponentRegistry.h"

namespace
{
}

GameBase::ComponentIndex& GameBase::ComponentRegistry::IndexCounter()
{
	static GameBase::ComponentIndex indexCounter_;  // 型Idを増やすやつ
	return indexCounter_;
}

std::vector<std::weak_ptr<void>>& GameBase::ComponentRegistry::PComponentPools()
{
	static std::vector<std::weak_ptr<void>> pComponentPools_;
	return pComponentPools_;
}

std::queue<std::function<void()>>& GameBase::ComponentRegistry::RegisterQueue()
{
	static std::queue<std::function<void()>> registerQueue_;
	return registerQueue_;
}

std::vector<std::string_view>& GameBase::ComponentRegistry::ComponentTypeNames()
{
	static std::vector<std::string_view> componentTypeNames_;
	return componentTypeNames_;
}

GameBase::ComponentIndex GameBase::ComponentRegistry::GetComponentIndex(const std::string_view _name)
{
	auto itr{ std::find(ComponentTypeNames().begin(), ComponentTypeNames().end(), _name)};
	GB_ASSERT(itr != ComponentTypeNames().end()
		&& "コンポーネント名で検索したが見つからなかった");

	return itr - ComponentTypeNames().begin();
}

GameBase::IComponentPool& GameBase::Get(const ComponentIndex _index)
{
	auto sp{ ComponentRegistry::PComponentPools()[_index].lock() };
	auto spIComponentPool{ std::static_pointer_cast<IComponentPool>(sp) };
	return *spIComponentPool.get();
}
