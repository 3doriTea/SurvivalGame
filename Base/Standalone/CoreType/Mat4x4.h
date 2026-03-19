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

		Mat4x4(
			double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23,
			double m30, double m31, double m32, double m33) noexcept :
			DirectX::XMMATRIX
			{
				static_cast<float>(m00), static_cast<float>(m01), static_cast<float>(m02), static_cast<float>(m03),
				static_cast<float>(m10), static_cast<float>(m11), static_cast<float>(m12), static_cast<float>(m13),
				static_cast<float>(m20), static_cast<float>(m21), static_cast<float>(m22), static_cast<float>(m23),
				static_cast<float>(m30), static_cast<float>(m31), static_cast<float>(m32), static_cast<float>(m33),
			}
		{}

		Mat4x4(
			const DirectX::XMVECTOR& row1 = {},
			const DirectX::XMVECTOR& row2 = {},
			const DirectX::XMVECTOR& row3 = {},
			const DirectX::XMVECTOR& row4 = {}) :
			DirectX::XMMATRIX
			{
				row1,
				row2,
				row3,
				row4,
			}
		{}

		/// <summary>
		/// 単位行列を取得する
		/// </summary>
		/// <returns>単位行列</returns>
		static inline Mat4x4 Identity() { return DirectX::XMMatrixIdentity(); }
	};
}
