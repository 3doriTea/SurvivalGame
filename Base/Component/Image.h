#pragma once
#include "../ComponentBase.h"
#include "../Structure/Material.h"


namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : 
	/// </summary>
	struct Image : ComponentBase<Image>
	{
		Image();
		void OnLoad(const YAML::Node& _node, SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, SchemaLoadBundle& _bundle) override;

		Color color;
		TextureHandle hTexture;
		MaterialHandle hMaterial;
	};
}
