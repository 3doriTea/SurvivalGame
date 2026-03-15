#pragma once
#include "ComponentIndex.h"
#include "ECS/IComponentPool.h"


namespace GameBase
{
	template<typename T>
	struct ComponentBase;

	template<typename T>
	class ComponentPool;

	namespace ComponentRegistry
	{
		ComponentIndex& IndexCounter();
		std::vector<std::weak_ptr<void>>& PComponentPools();
		std::queue<std::function<void()>>& RegisterQueue();
		std::vector<std::string_view>& ComponentTypeNames();

		/// <summary>
		/// コンポーネントインデクスを取得する
		/// </summary>
		/// <typeparam name="T">コンポーネントの型</typeparam>
		/// <returns>インデクス</returns>
		template<typename T>
		ComponentIndex GetComponentIndex();

		/// <summary>
		/// コンポーネントインデクスを取得する
		/// </summary>
		/// <returns>インデクス</returns>
		ComponentIndex GetComponentIndex(const std::string_view _name);

		template<typename T>
		size_t Add(std::weak_ptr<IComponentPool> _p, const std::string_view _name);
	}

	template<typename T>
	concept IsValidComponent = std::derived_from<T, ComponentBase<T>>;

	template<typename T>
		requires IsValidComponent<T>
	ComponentPool<T>& Get();

	IComponentPool& Get(const ComponentIndex _index);
}

#include "ComponentRegistry.inl"
