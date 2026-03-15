#pragma once
#include "FileIdSchema.h"

namespace GameBase::Schema
{
	struct GameScene
	{
		std::string name;
		std::vector<FileId> gameObjects;
	};
}
