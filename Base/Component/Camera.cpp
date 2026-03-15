#include "Camera.h"

GameBase::Component::Camera::Camera() :
	nearZ{ 0.1f },
	farZ{ 300.0f },
	fovRadian{ DirectX::XM_PIDIV4 },
	aspectRatio{},
	viewMatrix{},
	projectionMatrix{}
{
}

void GameBase::Component::Camera::OnLoad(const YAML::Node& _node)
{
		nearZ = _node["near"].as<float>(nearZ);
		farZ = _node["far"].as<float>(farZ);
		fovRadian = _node["fov"].as<float>(fovRadian);
}

void GameBase::Component::Camera::OnSave(YAML::Emitter& _emitter)
{
	_emitter << YAML::Key << "near" << YAML::Value << nearZ;
	_emitter << YAML::Key << "far" << YAML::Value << farZ;
	_emitter << YAML::Key << "fov" << YAML::Value << fovRadian;
}
