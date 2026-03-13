#pragma once
#include "../ComponentBase.h"
#include "../ECS/Entity.h"


namespace GameBase::Component
{
	struct Transform : ComponentBase<Transform>
	{
		Transform();

		Vec3 position;
		Quaternion rotation;
		Vec3 scale;
		Entity parent;
	};
}
