#pragma once
#include "../ComponentBase.h"
#include "../ECS/Entity.h"


namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : 座標系
	/// </summary>
	struct Transform : ComponentBase<Transform>
	{
		Transform();
		void OnLoad(const YAML::Node& _node) override;
		void OnSave(YAML::Emitter& _emitter) override;

		Vec3 position;
		Quaternion rotation;
		Vec3 scale;
		Entity parent;
	};
}
