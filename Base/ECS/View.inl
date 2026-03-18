#include "View.h"

template<typename ...ComponentsT>
inline GameBase::View<ComponentsT...>::View(EntityRegistry& _registry) :
	registry_{ _registry }
{}

template<typename ...ComponentsT>
inline void GameBase::View<ComponentsT...>::ForEach(std::function<void(Entity, ComponentsT&...)> _callback)
{
	// リクエストされたコンポーネントのマスクを求める
	Signature mask{};
	GetComponentsSignature<ComponentsT...>(mask);

	for (EntityIndex index = 0; index < registry_.entityCounter_; index++)
	{
		if (registry_.MatchComponents(index, mask))
		{
			_callback(
				MakeEntity(index, registry_.VERSION_),
				registry_.template GetComponent<ComponentsT>(index)...);
		}
	}
}

template<typename ...ComponentsT>
inline void GameBase::View<ComponentsT...>::RefAt(const Entity _entity, std::function<void(ComponentsT&...)> _callback)
{
	// リクエストされたコンポーネントのマスクを求める
	Signature mask{};
	GetComponentsSignature<ComponentsT...>(mask);
	EntityIndex index{ GetIndex(_entity) };

	GB_ASSERT(registry_.MatchComponents(index, mask)
		&& "指定したエンティティはコンポーネントマスクに一致しない");

	_callback(registry_.template GetComponent<ComponentsT>(index)...);
}

template<typename ...ComponentsT>
inline std::tuple<ComponentsT&...> GameBase::View<ComponentsT...>::At(const Entity _entity)
{
	// リクエストされたコンポーネントのマスクを求める
	Signature mask{};
	GetComponentsSignature<ComponentsT...>(mask);
	EntityIndex index{ GetIndex(_entity) };

	GB_ASSERT(registry_.MatchComponents(index, mask)
		&& "指定したエンティティはコンポーネントマスクに一致しない");

	return { registry_.template GetComponent<ComponentsT>(index)... };
}
