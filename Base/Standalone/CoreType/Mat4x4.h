#pragma once
#include <DirectXMath.h>

namespace GameBase
{
	struct Mat4x4 : DirectX::XMMATRIX
	{
		using DirectX::XMMATRIX::XMMATRIX;

		Mat4x4(DirectX::XMMATRIX&& _other) :
			DirectX::XMMATRIX{ std::move(_other) }
		{}

		Mat4x4(const DirectX::XMMATRIX& _other) :
			DirectX::XMMATRIX{ _other }
		{}

		Mat4x4() :
			DirectX::XMMATRIX{ DirectX::XMMatrixIdentity() }
		{}

		/// <summary>
		/// 単位行列を取得する
		/// </summary>
		/// <returns>単位行列</returns>
		static inline Mat4x4 Identity() { return DirectX::XMMatrixIdentity(); }
	};
}
