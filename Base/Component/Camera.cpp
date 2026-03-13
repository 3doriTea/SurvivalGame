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
	if (_node["Camera"])
	{
		const YAML::Node& component{ _node["Camera"] };
		nearZ = component["near"].as<float>();
		farZ = component["far"].as<float>();
		fovRadian = component["fov"].as<float>();
	}
}

void GameBase::Component::Camera::OnSave(YAML::Emitter& _emitter)
{
	_emitter << YAML::Key << "Camera";
	_emitter << YAML::Value << YAML::BeginMap;

	_emitter << YAML::Key << "near" << YAML::Value << nearZ;
	_emitter << YAML::Key << "far" << YAML::Value << farZ;

	_emitter << YAML::EndMap;
	_emitter << YAML::EndDoc;
}
