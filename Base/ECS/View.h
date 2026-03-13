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

		void ForEach(std::function<void(Entity, ComponentsT&...)> _callback);

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

			GetComponentsSignature<Rest...>(_signature);
		}

	private:
		EntityRegistry& registry_;  // レジストリ参照
	};
}

#include "View.inl"
