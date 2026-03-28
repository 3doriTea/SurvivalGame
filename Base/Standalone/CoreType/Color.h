#pragma once
#include <DirectXMath.h>

namespace GameBase
{
	/// <summary>
	/// 色構造体
	/// </summary>
	struct Color
	{
		enum
		{
			AT_R,
			AT_G,
			AT_B,
			AT_A,
			AT_MAX,
		};

		Color(
			const float _r,
			const float _g,
			const float _b,
			const float _a = 1.0f) :
			r{ _r },
			g{ _g },
			b{ _b },
			a{ _a }
		{}

		Color() :
			Color{ 0.0f, 0.0f, 0.0f }
		{}

		union
		{
			struct
			{
				float r;  // 赤色成分
				float g;  // 緑色成分
				float b;  // 青色成分
				float a;  // アルファ
			};
			DirectX::XMFLOAT4 xm;  // XMFLOAT4
			float f[AT_MAX];       // 配列としてアクセスする
		};

		static const Color WHITE;
	};

	inline const Color Color::WHITE{ 1, 1, 1 };
}
