#pragma once
#include "Structure/SchemaLoadBundle.h"


namespace GameBase
{
	/// <summary>
	/// コンポーネントのベースインタフェース
	/// </summary>
	struct IComponentBase
	{
		inline IComponentBase() = default;
		inline virtual ~IComponentBase() = default;

		virtual void OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle) = 0;
		virtual void OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle) = 0;
	};
}
