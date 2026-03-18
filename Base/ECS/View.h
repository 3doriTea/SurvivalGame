#pragma once
#include "Entity.h"
#include "../ComponentBase.h"
#include "Signature.h"

namespace GameBase
{
	class EntityRegistry;

	template<typename ...ComponentsT>
	class View
	{
	public:
		View(EntityRegistry& _registry);
		inline virtual ~View() = default;

		/// <summary>
		/// 指定コンポーネントを持つ全てのエンティティを走査
		/// </summary>
		/// <param name="_callback">void(Entity, ComponentsT&...)</param>
		void ForEach(std::function<void(Entity, ComponentsT&...)> _callback);

		/// <summary>
		/// エンティティからコンポーネントを参照
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_callback">void(ComponentsT&...)</param>
		void RefAt(const Entity _entity, std::function<void(ComponentsT&...)> _callback);

		/// <summary>
		///エンティティからコンポーネントを参照
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <returns>各コンポーネントの参照ポインタ</returns>
		std::tuple<ComponentsT&...> At(const Entity _entity);

	private:
		/// <summary>
		/// 再起停止用
		/// </summary>
		/// <param name="_signature">所有コンポーネントのフラグ</param>
		inline void GetComponentsSignature(Signature& _signature) {}
		/// <summary>
		/// 
		/// </summary>
		/// <typeparam name="FirstT"></typeparam>
		/// <typeparam name="...Rest"></typeparam>
		/// <param name="_signature"></param>
		template<typename FirstT, typename ...Rest>
		inline void GetComponentsSignature(Signature& _signature)
		{
			size_t index{ ComponentRegistry::GetComponentIndex<FirstT>() };
			_signature[index] = true;  // コンポーネントインデクスにフラグを立てる

			if constexpr (sizeof...(Rest) > 0)
			{
				GetComponentsSignature<Rest...>(_signature);
			}
			else
			{
				GetComponentsSignature(_signature);
			}

		}

	private:
		EntityRegistry& registry_;  // レジストリ参照
	};
}

#include "View.inl"
