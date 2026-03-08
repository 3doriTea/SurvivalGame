#pragma once
#include "ECS/IComponentPool.h"
#include "ECS/ComponentPool.h"


namespace GameBase
{
	using ComponentIndex = size_t;

	template<typename T>
	class ComponentBase;

	namespace ComponentRegistry
	{
		ComponentIndex& IndexCounter();
		std::vector<std::weak_ptr<void>>& PComponentPools();
		std::queue<std::function<void()>>& RegisterQueue();

		/// <summary>
		/// コンポーネントインデクスを取得する
		/// </summary>
		/// <typeparam name="T">コンポーネントの型</typeparam>
		/// <returns>インデクス</returns>
		template<typename T>
		ComponentIndex GetComponentIndex();

		template<typename T>
		size_t Add(std::weak_ptr<IComponentPool> _p);
	}

	template<typename T>
	concept IsValidComponent = std::derived_from<T, ComponentBase<T>>;

	template<typename T>
	requires IsValidComponent<T>
	ComponentPool<T>& Get();

	/// <summary>
	/// コンポーネントの基底クラス
	/// </summary>
	/// <typeparam name="T">コンポーネントの型</typeparam>
	template<typename T>
	class ComponentBase
	{
	public:
		ComponentBase();
		// NOTE: コンポーネントの登録にのみ使うため、VTableは不要
		inline /*virtual*/ ~ComponentBase() = default;



	private:
		static inline std::shared_ptr<ComponentPool<T>> pInstance_
		{
			std::make_shared<ComponentPool<T>>()
		};
	};
}

#include "ComponentBase.inl"
