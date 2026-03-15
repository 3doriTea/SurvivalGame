#pragma once
#include "ECS/IComponentBase.h"
#include "ECS/ComponentPool.h"


namespace GameBase
{
	/// <summary>
	/// コンポーネントの基底クラス
	/// </summary>
	/// <typeparam name="T">コンポーネントの型</typeparam>
	template<typename T>
	struct ComponentBase : IComponentBase
	{
		friend class ComponentPool<T>;
	public:
		// NOTE: Poolの方のコンストラクタが呼ばれるよ
		inline ComponentBase() = default;
		inline virtual ~ComponentBase() = default;

		virtual void OnLoad(const YAML::Node& _node) override = 0;
		virtual void OnSave(YAML::Emitter& _emitter) override = 0;

	private:
		inline static std::shared_ptr<ComponentPool<T>> pInstance_
		{
			std::make_shared<ComponentPool<T>>()
		};
	};
}

#include "ComponentBase.inl"
