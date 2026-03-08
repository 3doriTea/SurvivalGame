#include "ComponentBase.h"

template<typename T>
GameBase::ComponentIndex GameBase::ComponentRegistry::GetComponentIndex()
{
	static_assert(std::is_base_of_v<ComponentBase<T>, T>,
		"T must derive from ComponentBase<T>");

	static size_t index{ IndexCounter()++ };
	return index;
}

template<typename T>
size_t GameBase::ComponentRegistry::Add(std::weak_ptr<IComponentPool> _p)
{
	size_t index{ GetComponentIndex<T>() };
	while (index >= PComponentPools().size())
	{
		PComponentPools().push_back({});
	}

	PComponentPools().at(index) = _p;

	return index;
}

template<typename T>
requires GameBase::IsValidComponent<T>
GameBase::ComponentPool<T>& GameBase::Get()
{
	using namespace ComponentRegistry;
	size_t index{ GetComponentIndex<T>() };

	auto sp{ PComponentPools()[index].lock() };
	assert(sp && "コンポーネントインタフェースのシェアポインタ取得に失敗");

	return *static_cast<ComponentPool<T>*>(sp.get());
}

template<typename T>
inline GameBase::ComponentBase<T>::ComponentBase()
{
	ComponentRegistry::RegisterQueue().push([]()
	{
		ComponentRegistry::Add<T>(pInstance_);
	});
}
