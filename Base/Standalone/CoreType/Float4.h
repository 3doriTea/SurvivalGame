#pragma once
#include <DirectXMath.h>

namespace GameBase
{
	/// <summary>
	/// float 4つぶん
	/// </summary>
	struct Float4
	{
		enum
		{
			AT_X,
			AT_Y,
			AT_Z,
			AT_W,
			AT_MAX,
		};

		Float4() :
			Float4{ 0.0f, 0.0f, 0.0f, 0.0f }
		{}

		Float4(const DirectX::XMFLOAT4& _xm) :
			xm{ _xm }
		{}

		Float4(DirectX::XMFLOAT4&& _xm) :
			xm{ std::move(_xm) }
		{}

		Float4(const DirectX::XMVECTOR& _xmv)
		{
			DirectX::XMStoreFloat4(&xm, _xmv);
		}

		Float4(const float _x, const float _y, const float _z, const float _w) :
			x{ _x },
			y{ _y },
			z{ _z },
			w{ _w }
		{}
		
		union
		{
			float v[AT_MAX];
			DirectX::XMFLOAT4 xm;
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};
	};
}
