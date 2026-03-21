#pragma once
#include "../ComponentBase.h"


namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : 3Dモデル描画
	/// </summary>
	struct ModelRenderer : ComponentBase<ModelRenderer>
	{
		ModelRenderer();
		void OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle) override;

		ModelHandle hModel;  // モデルハンドル
	};
}
