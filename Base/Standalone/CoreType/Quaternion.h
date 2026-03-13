#pragma once
#include <DirectXMath.h>

namespace GameBase
{
	/// <summary>
	/// 四元数
	/// </summary>
	struct Quaternion : DirectX::XMFLOAT4
	{
		using DirectX::XMFLOAT4::XMFLOAT4;
	};
}

