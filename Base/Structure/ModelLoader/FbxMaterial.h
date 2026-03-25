#pragma once
#include <MaterialBase.h>

namespace GameBase
{
	struct FbxMaterialConstant
	{
		Float4 ambient;    // 環境光
		Float4 diffuse;    // 拡散反射光
		Float4 specular;   // 鏡面反射光

		Float4 shininess;  // 鏡面反射光の係数
	};

	/// <summary>
	/// Fbxモデルのマテリアル
	/// </summary>
	struct FbxMaterial : MaterialBase
	{
		FbxMaterial(const ShaderHandle _hShader, const FbxMaterialConstant& _constant) :
			hShader{ _hShader },
			hDiffuseTex{ INVALID_HANDLE },
			hNormalTex{ INVALID_HANDLE },
			constant_{ _constant }
		{}
		inline ~FbxMaterial() = default;

		inline size_t Size() const override { return sizeof(constant_); }
		inline const void* GetPtr() const override { return &constant_; }

		inline TextureHandle TextureDiffuse() const override { return hDiffuseTex; }
		inline TextureHandle TextureNormal() const override { return hNormalTex; }
		ShaderHandle Shader() const override { return hShader; }


		ShaderHandle hShader;  // シェーダハンドル

		TextureHandle hDiffuseTex;  // ディフューズテクスチャ
		TextureHandle hNormalTex;   // 法線マップ

		FbxMaterialConstant constant_;  // コンスタント
	};
}
