#pragma once
#include "FileIdSchema.h"

namespace GameBase::Schema
{
	struct GameComponent
	{
		FileId self;
		std::string tag;
		YAML::Node node;
	};
}
