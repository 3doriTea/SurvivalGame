#pragma once
#include <MaterialBase.h>

namespace GameBase
{
	struct FbxMaterial : MaterialBase
	{
		FbxMaterial() :
			hShader_{ INVALID_HANDLE },
			hDiffuseTex_{ INVALID_HANDLE },
			hNormalTex_{ INVALID_HANDLE },
			ambient{},
			diffuse{},
			specular{},
			shininess{}
		{}
		inline ~FbxMaterial() = default;

		inline size_t Size() const override { return sizeof(FbxMaterial); }

		inline TextureHandle TextureDiffuse() const override { return hDiffuseTex_; }
		inline TextureHandle TextureNormal() const override { return hNormalTex_; }


		ShaderHandle hShader_;

		TextureHandle hDiffuseTex_;  // ディフューズテクスチャ
		TextureHandle hNormalTex_;   // 法線マップ

		Float4 ambient;    // 環境光
		Float4 diffuse;    // 拡散反射光
		Float4 specular;   // 鏡面反射光

		Float4 shininess;  // 鏡面反射光の係数
	};
}
