#pragma once

namespace GameBase
{
	class ISystemBase;
	template<typename T>
	class SystemBase;

	namespace SystemRegistry
	{
		static inline size_t idCounter_;  // 型Idを増やすやつ
		std::vector<std::weak_ptr<ISystemBase>> pSystems_;

		/// <summary>
		/// システムIdを取得する
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		static size_t GetSystemId()
		{
			static_assert(std::is_base_of_v<SystemBase<T>, T> && std::is_base_of_v<ISystemBase, T>,
				"T must derive from both ISystemBase and SystemBase<T>");

			static size_t id{ idCounter_++ };
			return id;
		}

		template<typename T>
		static size_t Add(std::weak_ptr<ISystemBase> _p)
		{
			size_t id{ GetSystemId<T>() };
			while (id >= pSystems_.size())
			{
				pSystems_.push_back(nullptr);
			}

			pSystems_.at(id) = _p;// ここに SystemBase::pInstance_を入れたい

			return id;
		}

	}

	template<typename T>
	std::weak_ptr<T> Get()
	{
		using namespace SystemRegistry;

		size_t id{ GetSystemId<T>() };
		if (id >= pSystems_.size() || !pSystems_.at(id))
		{
			assert(false && "未登録のシステムが参照されました。");
			return nullptr;
		}

		 return std::static_pointer_cast<T>(pSystems_.at(id));
	}

	class ISystemBase
	{
	public:
		ISystemBase() = default;
		virtual ~ISystemBase() = default;

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Release() = 0;
	};

	template<typename T>
	class SystemBase : public ISystemBase
	{
	public:
		SystemBase() = default;
		virtual ~SystemBase() = default;

	private:
		static std::shared_ptr<T> pInstance_;
	};

	template<typename T>
	std::shared_ptr<T> SystemBase<T>::pInstance_{ std::make_shared<T>() };
}
