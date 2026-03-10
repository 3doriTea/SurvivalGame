#pragma once
#include "../ComponentBase.h"
#include "../ECS/Entity.h"


namespace GameBase::Component
{
	struct Transform : ComponentBase<Transform>
	{
		Transform();

		Vec3 position;
		DirectX::XMFLOAT4 rotation;
		Vec3 scale;
		Entity parent;
	};
}
