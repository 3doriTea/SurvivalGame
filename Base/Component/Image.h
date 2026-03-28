#pragma once
#include "../ComponentBase.h"


namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : 
	/// </summary>
	struct Image : ComponentBase<Image>
	{
		Image();
		void OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle) override;

		Color color;
		TextureHandle hTexture;
		MaterialHandle hMaterial;
	};
}
