#pragma once
#include <Windows.h>


namespace GameBase
{
	namespace Util
	{
		static inline const UINT PERIOD_MILLI{ 1 };                // 分解能 (ミリ秒)
		static inline const LONGLONG ONE_SEC_TO_MICRO{ 1000000 };  // 1秒をマイクロ秒で表す
		static inline const float MICRO_TO_SEC{ 0.0000001f };      // 1マイクロ秒を秒で表す
	}
}
