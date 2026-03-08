#pragma once

namespace GameBase
{
	/// <summary>
	/// 画面のリフレッシュレート
	/// </summary>
	struct RefreshRate
	{
		/// <summary>
		/// FPSでリフレッシュレートを指定
		/// </summary>
		/// <param name="fps">Frame par sec</param>
		RefreshRate(const uint32_t fps = 60) :
			denominator{ 1 },
			numerator{ fps }
		{
		}
		uint32_t denominator;  // 分子
		uint32_t numerator;  // 分母
	};
}
