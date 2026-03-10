#pragma once
#include "SerializedId.h"

namespace GameBase::Serialized
{
	struct Scene
	{
		std::string name = "TEST";
		std::vector<Id> gameObjects
		{
			1,
			2,
			3,
			4,
			5,
			120,
		};
	};
}
