#include "Transform.h"


GameBase::Component::Transform::Transform():
	position{},
	rotation{},
	scale{},
	parent{}
{
}

void GameBase::Component::Transform::OnLoad(const YAML::Node& _node)
{
	if (_node["Transform"])
	{
		const YAML::Node& component{ _node["Transform"] };
		position.x = component["position"]["x"].as<float>();
		position.y = component["position"]["y"].as<float>();
		position.z = component["position"]["z"].as<float>();

		rotation.x = component["rotation"]["x"].as<float>();
		rotation.y = component["rotation"]["y"].as<float>();
		rotation.z = component["rotation"]["z"].as<float>();
		rotation.w = component["rotation"]["w"].as<float>();

		scale.x = component["scale"]["x"].as<float>();
		scale.y = component["scale"]["y"].as<float>();
		scale.z = component["scale"]["z"].as<float>();

		parent = component["parent"].as<EntityIndex>();
	}
}

void GameBase::Component::Transform::OnSave(YAML::Emitter & _emitter)
{
	_emitter << YAML::Key << "Camera";
	_emitter << YAML::Value << YAML::BeginMap;

	_emitter << YAML::Key << "position";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << position.x;
	_emitter << YAML::Key << "y" << YAML::Value << position.x;
	_emitter << YAML::Key << "z" << YAML::Value << position.x;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "rotation";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << rotation.x;
	_emitter << YAML::Key << "y" << YAML::Value << rotation.y;
	_emitter << YAML::Key << "z" << YAML::Value << rotation.z;
	_emitter << YAML::Key << "w" << YAML::Value << rotation.w;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "scale";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << scale.x;
	_emitter << YAML::Key << "y" << YAML::Value << scale.x;
	_emitter << YAML::Key << "z" << YAML::Value << scale.x;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "parent" << YAML::Value << parent;

	_emitter << YAML::EndMap;
	_emitter << YAML::EndDoc;
}
