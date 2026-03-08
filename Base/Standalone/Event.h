#pragma once
#include <functional>
#include <cstdint>
#include <memory>
#include <vector>


namespace GameBase
{
	/// <summary>
	/// 複数登録できるイベント
	/// </summary>
	/// <typeparam name="...Args">可変長引数</typeparam>
	template<typename ...Args>
	class Event
	{
	public:
		using Id = uint32_t;
		using Handler = std::function<void(Args...)>;

		struct Slot
		{
			Id id;
			Handler handler;
		};

	public:
		Event() : nextIdCounter_{ 0 } {};
		~Event() = default;

		inline [[nodiscard]] std::shared_ptr<void> Connect(Handler handler)
		{
			Id id{ nextIdCounter_++ };
			slots_.push_back({ id, std::move(handler) });

			// RAIIオブジェクトを作る
			std::shared_ptr<void> sentinel
			{
				std::shared_ptr<void>(nullptr, [this, id](void*)
				{
					Disconnect(id);
				})
			};

			return sentinel;
		}

		inline void Invoke(Args... args)
		{
			for (Slot& slot : slots_)
			{
				slot.handler(args...);
			}
		}

	private:
		inline void Disconnect(const Id _id)
		{
			auto itr = std::remove_if(slots_.begin(), slots_.end(), [_id](const auto& s)
			{
				return s.id == _id;
			});

			if (itr != slots_.end())
			{
				slots_.erase(itr, slots_.end());
			}
		}

	private:
		std::vector<Slot> slots_;
		Id nextIdCounter_;
	};
}
