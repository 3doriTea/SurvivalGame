#pragma once
#include "../ComponentBase.h"


namespace GameBase::Component
{
	static constexpr size_t GAME_OBJECT_NAME_BUFFER_SIZE{ 20 };

	/// <summary>
	/// コンポーネント : 
	/// </summary>
	struct GameObject : ComponentBase<GameObject>
	{
		GameObject();
		void OnLoad(const YAML::Node& _node, SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, SchemaLoadBundle& _bundle) override;

		void SetName(const std::string_view _name);

		std::array<char, GAME_OBJECT_NAME_BUFFER_SIZE> name;  // 名前
	};
}
