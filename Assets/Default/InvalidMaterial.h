#pragma once
#include <MaterialBase.h>

using namespace GameBase;

struct InvalidMaterial : MaterialBase
{
	inline size_t Size() const override { return sizeof(InvalidMaterial); }
	inline const void* GetPtr() const override { return nullptr; }
	inline ShaderHandle Shader() const override { return INVALID_HANDLE; }
};
