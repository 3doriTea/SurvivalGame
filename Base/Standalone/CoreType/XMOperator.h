#pragma once
#include "Mat4x4.h"
#include "Vec3.h"

namespace GameBase
{
	inline Vec3 operator*(const Vec3& _v, const Mat4x4& _m)
	{
		using namespace DirectX;

		return Vec3{ XMVector3TransformCoord(_v, _m) };
	}
}
