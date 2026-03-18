#pragma once
#include "ECS/EntityRegistry.h"


namespace GameBase
{
	using SystemIndex = size_t;

	class ISystemBase;
	struct ISystemInterfaceBase;
	template<typename T, typename InterfaceT>
	class SystemBase;

	namespace SystemRegistry
	{
		/// <summary>
		/// システムのインデクスカウンタ
		/// </summary>
		SystemIndex& IndexCounter();
		/// <summary>
		/// システム本体格納
		/// </summary>
		std::vector<std::weak_ptr<ISystemBase>>& PSystems();
		/// <summary>
		/// システムのインタフェース格納
		/// </summary>
		std::vector<std::weak_ptr<void>>& PInterfaces();
		/// <summary>
		/// コンストラクタ実行後に呼び出される処理
		/// </summary>
		std::queue<std::function<void()>>& RegisterQueue();
		std::queue<std::function<void()>>& DestructionQueue();

		/// <summary>
		/// システムインデクスを取得する
		/// </summary>
		/// <typeparam name="T">システムの型</typeparam>
		/// <returns>インデクス</returns>
		template<typename T>
		SystemIndex GetSystemIndex();

		template<typename T>
		size_t Add(std::weak_ptr<ISystemBase> _p);
	}

	template<typename T>
	concept IsValidSystem = std::derived_from<T, ISystemBase> &&
		requires{ typename T::Interface; };

	/// <summary>
	/// システムの参照をするゲッタ
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	requires IsValidSystem<T>
	typename T::Interface& Get();

	namespace System
	{
		/// <summary>
		/// システムの参照をするゲッタ
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		requires IsValidSystem<T>
		T& Get();
	}

	struct ISystemInterfaceBase
	{
		inline ISystemInterfaceBase() = default;
		inline virtual ~ISystemInterfaceBase() = default;
	};

	class ISystemBase
	{
	public:
		inline ISystemBase() = default;
		inline virtual ~ISystemBase() = default;

		virtual void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) = 0;
		virtual void Initialize() = 0;
		virtual void Update(EntityRegistry&) = 0;
		virtual void Release() = 0;
	};

	template<typename T, typename InterfaceT>
	class SystemBase : public ISystemBase, public InterfaceT
	{
	public:
		using Interface = InterfaceT;

	public:
		SystemBase();
		inline virtual ~SystemBase() = default;

		inline virtual void OnRegisterDependencies(
			FluentVectorAddOnly<SystemIndex>* _pRegistry) override {}

	private:
		static inline std::shared_ptr<T> pInstance_{ std::make_shared<T>() };
	};
}

#include "SystemBase.inl"
