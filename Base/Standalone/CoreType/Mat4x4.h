#pragma once
#include <DirectXMath.h>

namespace GameBase
{
	struct Mat4x4 : DirectX::XMMATRIX
	{
		using DirectX::XMMATRIX::XMMATRIX;

		Mat4x4() :
			DirectX::XMMATRIX{ DirectX::XMMatrixIdentity() }
		{
		}
	};
}
