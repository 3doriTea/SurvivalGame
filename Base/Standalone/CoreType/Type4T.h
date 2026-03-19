#pragma once
#include <DirectXMath.h>

namespace GameBase
{
	/// <summary>
	/// プリミティブ型 4つぶん
	/// </summary>
	template<typename T, typename XMT>
	struct Type4
	{
		enum
		{
			AT_X,
			AT_Y,
			AT_Z,
			AT_W,
			AT_MAX,
		};

		Type4() :
			Type4{ 0.0f, 0.0f, 0.0f, 0.0f }
		{}

		Type4(const XMT& _xm) :
			xm{ _xm }
		{}

		Type4(XMT&& _xm) :
			xm{ std::move(_xm) }
		{}

		Type4(const DirectX::XMVECTOR& _xmv) requires std::is_same_v<T, float>
		{
			DirectX::XMStoreFloat4(&xm, _xmv);
		}

		Type4(const T _x, const T _y, const T _z, const T _w) :
			x{ _x },
			y{ _y },
			z{ _z },
			w{ _w }
		{}

		union
		{
			T v[AT_MAX];
			XMT xm;
			struct
			{
				T x;
				T y;
				T z;
				T w;
			};
		};
	};
}
