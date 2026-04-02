#pragma once
#include "../ComponentBase.h"


namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : スクリプト
	/// </summary>
	struct Script : ComponentBase<Script>
	{
		Script();
		void OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle) override;

		ScriptHandle hScript;  // スクリプト
	};
}
