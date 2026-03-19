#pragma once
#include <MaterialBase.h>

namespace GameBase
{
	struct FbxMaterial : MaterialBase
	{
		FbxMaterial() :
		{}
		inline ~FbxMaterial() = default;

		inline size_t Size() const override { return sizeof(FbxMaterial); }


		ShaderHandle hShader_;
		TextureHandle hTexture_;
		
	};
}
