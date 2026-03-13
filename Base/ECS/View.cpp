#include "View.h"
#include "EntityRegistry.h"

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
				registry_.GetComponent<ComponentsT>(index)...
			);
		}
	}
}
