#include "Transform.h"
#include <System/TransformCalculator.h>


GameBase::Component::Transform::Transform():
	position{},
	rotation{},
	scale{},
	parent{},
	localTranslateMatrix{ Mat4x4::Identity() },
	localRotationMatrix{ Mat4x4::Identity() },
	localScaleMatrix{ Mat4x4::Identity() },
	worldMatrix{ Mat4x4::Identity() }
{
}

void GameBase::Component::Transform::OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle)
{
	position.x = _node["position"]["x"].as<float>(position.x);
	position.y = _node["position"]["y"].as<float>(position.y);
	position.z = _node["position"]["z"].as<float>(position.z);

	Vec3 euler{ rotation.ToEuler() };

	euler.x = _node["rotation"]["x"].as<float>(euler.x);
	euler.y = _node["rotation"]["y"].as<float>(euler.y);
	euler.z = _node["rotation"]["z"].as<float>(euler.z);

	rotation = Quaternion::FromEuler(euler);

	scale.x = _node["scale"]["x"].as<float>(scale.x);
	scale.y = _node["scale"]["y"].as<float>(scale.y);
	scale.z = _node["scale"]["z"].as<float>(scale.z);

	parent = _bundle.fileIdToEntity.at(_node["parent"]["fileId"].as<Schema::FileId>(0));
}

void GameBase::Component::Transform::OnSave(YAML::Emitter & _emitter, const SchemaLoadBundle& _bundle)
{
	_emitter << YAML::Key << "position";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << position.x;
	_emitter << YAML::Key << "y" << YAML::Value << position.y;
	_emitter << YAML::Key << "z" << YAML::Value << position.z;
	_emitter << YAML::EndMap;

	Vec3 euler{ rotation.ToEuler() };

	_emitter << YAML::Key << "rotation";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << euler.x;
	_emitter << YAML::Key << "y" << YAML::Value << euler.y;
	_emitter << YAML::Key << "z" << YAML::Value << euler.z;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "scale";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << scale.x;
	_emitter << YAML::Key << "y" << YAML::Value << scale.y;
	_emitter << YAML::Key << "z" << YAML::Value << scale.z;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "parent";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "fileId";
	_emitter << YAML::Value << _bundle.entityToFileId.at(parent);
	_emitter << YAML::EndMap;
	_emitter << YAML::EndMap;
}
