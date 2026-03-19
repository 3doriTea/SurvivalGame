#pragma once
#include <DirectXMath.h>

namespace GameBase
{
	/// <summary>
	/// ジンバルロック回避のためのしきいち
	/// </summary>
	static inline const float GIMBAL_LOCK_THRESHOLD{ 0.0001f };

	/// <summary>
	/// 四元数
	/// </summary>
	struct Quaternion : DirectX::XMFLOAT4
	{
		using DirectX::XMFLOAT4::XMFLOAT4;

		Quaternion(const DirectX::XMVECTOR& _v)
		{
			DirectX::XMStoreFloat4(this, _v);
		}

		inline operator DirectX::XMVECTOR() const noexcept
		{
			return DirectX::XMLoadFloat4(this);
		}

		inline DirectX::XMVECTOR ToEuler() const
		{
			using namespace DirectX;

			XMMATRIX m{ DirectX::XMMatrixRotationQuaternion(*this) };
			XMFLOAT4X4 matF44{};
			XMStoreFloat4x4(&matF44, m);

			float pitch{ asinf(-matF44._32) };
			float yaw{};
			float roll{};

			if (cosf(pitch) > GIMBAL_LOCK_THRESHOLD)
			{
				yaw = atan2f(matF44._31, matF44._33);
				roll = atan2f(matF44._12, matF44._22);
			}
			else
			{
				yaw = 0.0f;
				roll = atan2f(-matF44._21, matF44._11);
			}

			return { pitch, yaw, roll, 0.0f };
		}

		static inline Quaternion FromEuler(const DirectX::XMVECTOR& _euler)
		{
			DirectX::XMFLOAT3 eulerF3{};
			DirectX::XMStoreFloat3(&eulerF3, _euler);
			return DirectX::XMQuaternionRotationRollPitchYaw(
				eulerF3.x, eulerF3.y, eulerF3.z);
		}
	};
}

