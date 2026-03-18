#include "Transform.h"
#include <System/TransformCalculator.h>


GameBase::Component::Transform::Transform():
	position{},
	rotation{},
	scale{},
	parent{},
	localTranslateMatrix{},
	localRotationMatrix{},
	localScaleMatrix{},
	worldMatrix{}
{
}

void GameBase::Component::Transform::OnLoad(const YAML::Node& _node, SchemaLoadBundle& _bundle)
{
	position.x = _node["position"]["x"].as<float>(position.x);
	position.y = _node["position"]["y"].as<float>(position.y);
	position.z = _node["position"]["z"].as<float>(position.z);

	rotation.x = _node["rotation"]["x"].as<float>(rotation.x);
	rotation.y = _node["rotation"]["y"].as<float>(rotation.y);
	rotation.z = _node["rotation"]["z"].as<float>(rotation.z);
	rotation.w = _node["rotation"]["w"].as<float>(rotation.w);

	scale.x = _node["scale"]["x"].as<float>(scale.x);
	scale.y = _node["scale"]["y"].as<float>(scale.y);
	scale.z = _node["scale"]["z"].as<float>(scale.z);

	parent = _bundle.fileIdToEntity.at(_node["parent"].as<Schema::FileId>(0));
}

void GameBase::Component::Transform::OnSave(YAML::Emitter & _emitter, SchemaLoadBundle& _bundle)
{
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

	_emitter << YAML::Key << "parent" << YAML::Value << _bundle.entityToFileId.at(parent);
}
