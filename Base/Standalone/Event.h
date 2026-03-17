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
	class Event : public std::enable_shared_from_this<Event<Args...>>
	{
		Event(const Event& _other) = delete;             // コピーコンストラクタは消す
		inline Event& operator=(const Event& _other) = delete;  // コピーを消す
	public:
		using Id = uint32_t;
		using Handler = std::function<void(Args...)>;

		struct Slot
		{
			Id id;
			Handler handler;
		};

	protected:
		// 直接インスタンスさせない！
		Event() : nextIdCounter_{ 0 } {}

	public:
		/// <summary>
		/// イベントを作成
		/// </summary>
		/// <returns></returns>
		inline static std::shared_ptr<Event<Args...>> Create()
		{
			// NOTE: protected にしたため、make_sharedは使えない
			return std::shared_ptr<Event<Args...>>{ new Event<Args...>() };
		}

	public:
		~Event() = default;

		inline [[nodiscard]] std::shared_ptr<void> Connect(Handler handler)
		{
			Id id{ nextIdCounter_++ };
			slots_.push_back({ id, std::move(handler) });

			// 自分自身のweak_ptrを持ってくる
			std::weak_ptr<Event<Args...>> weakSelf
			{
				this->shared_from_this()
			};

			// RAIIオブジェクトを作る
			std::shared_ptr<void> sentinel
			{
				std::shared_ptr<void>(nullptr, [weakSelf, id](void*)
				{
					if (auto self{ weakSelf.lock() })
					{
						self.get()->Disconnect(id);
					}
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

	template<typename ...Args>
	using EventSubject = std::shared_ptr<Event<Args...>>;
}
