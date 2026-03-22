#pragma once
#include "RunMode.h"

namespace GameBase
{
	/// <summary>
	/// ワールド実行時情報
	/// </summary>
	struct RunInfo
	{
		RunInfo(
			RunMode _mode,
			std::string_view _sceneName) :
			mode{ _mode },
			sceneName{ _sceneName }
		{}

		RunMode mode;
		std::string_view sceneName;
	};
}
