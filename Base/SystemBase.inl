template<typename T>
size_t GameBase::SystemRegistry::GetSystemIndex()
{
	static_assert(std::is_base_of_v<SystemBase<T, typename T::Interface>, T> && std::is_base_of_v<ISystemBase, T>,
		"T must derive from both ISystemBase and SystemBase<T>");

	static size_t index{ IndexCounter()++};
	return index;
}

template<typename T>
size_t GameBase::SystemRegistry::Add(std::weak_ptr<ISystemBase> _p)
{
	size_t index{ GetSystemIndex<T>() };
	while (index >= PSystems().size())
	{
		PSystems().push_back({});
		PInterfaces().push_back({});
	}

	PSystems().at(index) = _p;// ここに SystemBase::pInstance_を入れたい

	auto spBase{ _p.lock() };
	GB_ASSERT(spBase && "システムベースのシェアポインタにアクセスできなかった");

	auto sp{ std::static_pointer_cast<T>(spBase) };
	GB_ASSERT(sp && "T型のシェアポインタにアクセスできなかった");

	PInterfaces().at(index) = std::static_pointer_cast<typename T::Interface>(sp);

	return index;
}

template<typename T>
requires GameBase::IsValidSystem<T>
typename T::Interface& GameBase::Get()
{
	using namespace SystemRegistry;
	size_t index{ GetSystemIndex<T>() };

	auto sp{ PInterfaces()[index].lock() };
	GB_ASSERT(sp && "システムインタフェースのシェアポインタ取得に失敗");

	return *static_cast<T::Interface*>(sp.get());
}

template<typename T>
requires GameBase::IsValidSystem<T>
T& GameBase::System::Get()
{
	using namespace SystemRegistry;
	size_t index{ GetSystemIndex<T>() };

	auto sp{ PSystems()[index].lock() };
	GB_ASSERT(sp && "システムインタフェースのシェアポインタ取得に失敗");

	return *std::static_pointer_cast<T>(sp).get();
}

template<typename T, typename InterfaceT>
inline GameBase::SystemBase<T, InterfaceT>::SystemBase()
{
	SystemRegistry::RegisterQueue().push([]()
	{
		size_t index{ SystemRegistry::Add<T>(pInstance_) };
		Debugger::LogF("追加 {}:{}", index, typeid(T).name());
	});
}
