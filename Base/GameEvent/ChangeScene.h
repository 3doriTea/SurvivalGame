#pragma once
#include "../GameEvent.h"

namespace GameBase::GameEvent
{
	/// <summary>
	/// シーンを遷移
	/// </summary>
	struct ChangeScene : GameEventBase
	{
		ChangeScene(const fs::path& _nextSceneFile) :
			nextSceneFile{ _nextSceneFile }
		{}
		fs::path nextSceneFile;
	};
}
