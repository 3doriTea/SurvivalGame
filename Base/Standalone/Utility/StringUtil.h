#pragma once
#include <string_view>


namespace GameBase
{
	namespace StringUtil
	{
		/// <summary>
		/// 文字列の始まりが一致しているか
		/// </summary>
		/// <param name="_src">調べられる文字列</param>
		/// <param name="_target">調べる文字列</param>
		/// <returns>一致している true / false</returns>
		static inline bool StartWith(
			const std::string_view _src,
			const std::string_view _target)
		{
			// ソースのサイズより大きいなら確実に不一致
			if (_src.size() < _target.size())
			{
				return false;
			}
			for (int i = 0; i < _target.size(); i++)
			{
				if (_src.data()[i] != _target.data()[i])
				{
					return false;
				}
			}
			return true;
		}
	}
}
