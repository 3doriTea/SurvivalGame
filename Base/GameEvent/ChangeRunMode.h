#pragma once
#include "../GameEvent.h"
#include "../Structure/RunMode.h"

namespace GameBase::GameEvent
{
	/// <summary>
	/// 実行モードを変更
	/// </summary>
	struct ChangeRunMode : GameEventBase
	{
		ChangeRunMode(const RunMode _runMode) :
			runMode{ _runMode }
		{}
		RunMode runMode;
	};
}
