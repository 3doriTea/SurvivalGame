#include "ScriptBase.h"
#include "ComponentBase.h"
#include "ECS/EntityRegistry.h"


template<typename T>
GameBase::ScriptIndex GameBase::ScriptRegistry::GetScriptIndex()
{
	static_assert(std::is_base_of_v<ScriptBase, T> &&
		"T must derive from both ScriptBase");

	static size_t index{ IndexCounter()++ };
	return index;
}

template<typename T>
size_t GameBase::ScriptRegistry::Add(std::weak_ptr<IScriptBase> _p)
{
	size_t index{ GetScriptIndex<T>() };
	while (index >= PScripts().size())
	{
		PScripts().push_back({});
	}

	PScripts().at(index) = _p;

	return size_t();
}

template<typename T>
T& GameBase::Script::Get()
{
	using namespace ScriptRegistry;
	size_t index{ GetScriptIndex<T>() };

	auto sp{ PScripts()[index].lock() };
	GB_ASSERT(sp && "スクリプトのシェアポインタ取得に失敗");

	return *std::static_pointer_cast<T>(sp).get();
}

template<typename T, typename DataT>
inline GameBase::ScriptBase<T, DataT>::ScriptBase() :
	self_{ INVALID_ENTITY },
	pRegistry_{ nullptr }
{
	// TODO: 登録処理
}

//template<typename T, typename DataT>
//template<typename ComponentT>
//inline ComponentT& GameBase::ScriptBase<T, DataT>::Get()
//requires std::derived_from<ComponentT, GameBase::IComponentBase>
//{
//	GB_ASSERT(pRegistry_ && "レジストリが未指定");
//	if (pRegistry_)
//	{
//		return *pRegistry_;
//	}
//	else
//	{
//		return ComponentT{};
//	}
//}

//template<typename T, typename DataT>
//template<typename SystemT>
//inline SystemT& GameBase::ScriptBase<T, DataT>::Get()
//requires std::derived_from<SystemT, GameBase::ISystemBase>
//{
//	return System::Get<T>();
//}
