#pragma once

namespace GameBase
{
	struct GameEventBase
	{
		inline GameEventBase() = default;
		virtual inline ~GameEventBase() = default;
	};

	namespace GameEvent
	{
		std::map<std::type_index, std::unique_ptr<GameEventBase>>& GetEventRegistry();

		template<typename T, typename ...Args>
		requires std::constructible_from<T, Args...>
		void Emplace(Args&& ..._args);

		template<typename T>
		bool TryErase(const std::function<void(T&)>& _callback = [](T&) {});
	}

}

#include "GameEvent.inl"
