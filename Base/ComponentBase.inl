#include "ComponentBase.h"

// Static definition for ComponentBase<T>::pInstance_
//template<typename T>
//inline std::shared_ptr<GameBase::ComponentPool<T>> GameBase::ComponentBase<T>::pInstance_
//{
//	std::make_shared<GameBase::ComponentPool<T>>()
//};

//template<typename T>
//inline GameBase::ComponentBase<T>::ComponentBase()
//{
//	static bool once
//	{
//		([]() -> bool
//			{
//				ComponentRegistry::RegisterQueue().push([]()
//				{
//					ComponentRegistry::Add<T>(pInstance_, typeid(T).name());
//				});
//
//				return true;
//			}) ()
//	};
//}
