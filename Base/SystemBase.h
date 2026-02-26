#pragma once

namespace GameBase
{
	class ISystemBase;

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
			static size_t id{ idCounter_++ };
			return id;
		}

		template<typename T>
		static size_t Add(ISystemBase* _p)
		{
			size_t id{ GetSystemId<T>() };
			while (id >= pSystems_.size())
			{
				pSystems_.push_back(nullptr);
			}

			pSystems_.at(id) = // ここに SystemBase::pInstance_を入れたい

			return id;
		}

	}

	template<typename T>
	std::weak_ptr<T> Get()
	{

	}

	class ISystemBase
	{
	public:
		ISystemBase() = default;
		virtual ~ISystemBase() = default;
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
