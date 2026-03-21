#pragma once
#include "../../ECS/EntityRegistry.h"

namespace GameBase::Editor
{
	/// <summary>
	/// コンポーネントコンボボックス一覧を表示する
	/// </summary>
	static inline bool ShowAllComponentsCombo(EntityRegistry& _registry)
	{
		for (ComponentIndex i = 0; i < ComponentRegistry::IndexCounter(); i++)
		{
			const std::string_view typeName{ ComponentRegistry::ComponentTypeNames()[i] };
		}
	}
}


