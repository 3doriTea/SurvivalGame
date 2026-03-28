#pragma once
#include <MaterialBase.h>

namespace GameBase
{
	struct UIMaterialConstant
	{
		Color color;  // 色
	};

	/// <summary>
	/// UIのマテリアル
	/// </summary>
	struct UIMaterial : MaterialBase
	{
		UIMaterial(
			const ShaderHandle _hShader,
			const UIMaterialConstant& _constant,
			const TextureHandle _hImageTex = INVALID_HANDLE) :
			hShader{ _hShader },
			constant{ _constant },
			hImageTex{ _hImageTex }
		{}

		inline ~UIMaterial() = default;

		inline size_t Size() const override { return sizeof(UIMaterialConstant); }
		inline const void* GetPtr() const override { return &constant; }

		inline TextureHandle TextureDiffuse() const override { return hImageTex; }
		inline TextureHandle TextureNormal() const override { return INVALID_HANDLE; }
		ShaderHandle Shader() const override { return hShader; }

		ShaderHandle hShader;
		TextureHandle hImageTex;
		UIMaterialConstant constant;
	};
}
