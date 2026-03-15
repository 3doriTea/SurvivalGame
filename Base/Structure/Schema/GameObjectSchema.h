#pragma once
#include "../ECS/Entity.h"
#include "FileIdSchema.h"

namespace GameBase::Schema
{
	/// <summary>
	/// ゲームオブジェクト
	/// </summary>
	struct GameObject
	{
		Entity entity;
		FileId self;
		std::string name;
		std::vector<FileId> gameComponents;
	};
}
