#include "Camera.h"
#include <System/Graphic/MainCamera.h>


GameBase::Component::Camera::Camera() :
	nearZ{ 0.1f },
	farZ{ 300.0f },
	fovRadian{ DirectX::XM_PIDIV4 },
	aspectRatio{ 1600.0f / 900.0f },
	viewMatrix{ Mat4x4::Identity() },
	projectionMatrix{ Mat4x4::Identity() }
{
}

void GameBase::Component::Camera::OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle)
{
		nearZ = _node["near"].as<float>(nearZ);
		if (nearZ <= FLT_EPSILON)
		{
			nearZ = 0.00001f;  // 0以下にさせない
		}
		farZ = _node["far"].as<float>(farZ);
		if (farZ <= FLT_EPSILON)
		{
			farZ = 0.00001f;  // 0以下にさせない
		}

		if (nearZ <= farZ)
		{
			farZ = nearZ + 0.00002f;
		}
		fovRadian = _node["fov"].as<float>(fovRadian);
}

void GameBase::Component::Camera::OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle)
{
	_emitter << YAML::Key << "near" << YAML::Value << nearZ;
	_emitter << YAML::Key << "far" << YAML::Value << farZ;
	_emitter << YAML::Key << "fov" << YAML::Value << fovRadian;
}
