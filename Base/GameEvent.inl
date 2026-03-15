#include "GameEvent.h"

template<typename T, typename ...Args>
requires std::constructible_from<T, Args...>
void GameBase::GameEvent::Emplace(Args && ..._args)
{
	GetEventRegistry().emplace(
		std::type_index{ typeid(T) },
		std::make_unique<T>(std::forward<Args>(_args)...));
}

template<typename T>
bool GameBase::GameEvent::TryErase(const std::function<void(T&)>& _callback)
{
	auto& registry{ GetEventRegistry() };
	auto itr{ registry.find(std::type_index{ typeid(T) })};
	if (itr != registry.end())
	{
		_callback(*static_cast<T*>(itr->second.get()));
		registry.erase(itr);
		return true;
	}
	return false;
}
