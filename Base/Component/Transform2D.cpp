#include "Transform2D.h"

GameBase::Component::Transform2D::Transform2D() :
	parent{ INVALID_ENTITY },
	position{ 0, 0 },
	angle{ 0.0f },
	scale{ 1, 1 },
	anchorPoint{ 0, 0 },
	pivotPoint{ 0, 0 },
	localMatrix{ Mat4x4::Identity() },
	worldMatrix{ Mat4x4::Identity() },
	toChildMatrix{ Mat4x4::Identity() }
{
}

void GameBase::Component::Transform2D::OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle)
{
	std::string t = YAML::Dump(_node);

	parent = _bundle.fileIdToEntity.at(_node["parent"]["fileId"].as<Schema::FileId>(0));

	position.x = _node["position"]["x"].as<float>(position.x);
	position.y = _node["position"]["y"].as<float>(position.y);

	angle = _node["angle"].as<float>(angle);
	angle = std::fmodf(angle, DirectX::XM_2PI);

	scale.x = _node["scale"]["x"].as<float>(scale.x);
	scale.y = _node["scale"]["y"].as<float>(scale.y);

	anchorPoint.x = _node["anchorPoint"]["x"].as<float>(anchorPoint.x);
	anchorPoint.y = _node["anchorPoint"]["y"].as<float>(anchorPoint.y);
	anchorPoint.x = std::clamp(anchorPoint.x, 0.0f, 1.0f);
	anchorPoint.y = std::clamp(anchorPoint.y, 0.0f, 1.0f);
	

	pivotPoint.x = _node["pivotPoint"]["x"].as<float>(pivotPoint.x);
	pivotPoint.y = _node["pivotPoint"]["y"].as<float>(pivotPoint.y);
	pivotPoint.x = std::clamp(pivotPoint.x, 0.0f, 1.0f);
	pivotPoint.y = std::clamp(pivotPoint.y, 0.0f, 1.0f);
}

void GameBase::Component::Transform2D::OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle)
{
	_emitter << YAML::Key << "parent";
	_emitter << YAML::Value
		<< YAML::BeginMap;
		_emitter << YAML::Key << "fileId";
		_emitter << YAML::Value << _bundle.entityToFileId.at(parent);
		_emitter << YAML::EndMap;

	_emitter << YAML::Key << "position";
	_emitter << YAML::Value << YAML::BeginMap;
		_emitter << YAML::Key << "x" << YAML::Value << position.x;
		_emitter << YAML::Key << "y" << YAML::Value << position.y;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "angle";
	_emitter << YAML::Value << angle;

	_emitter << YAML::Key << "scale";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << scale.x;
	_emitter << YAML::Key << "y" << YAML::Value << scale.y;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "anchorPoint";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << anchorPoint.x;
	_emitter << YAML::Key << "y" << YAML::Value << anchorPoint.y;
	_emitter << YAML::EndMap;

	_emitter << YAML::Key << "pivotPoint";
	_emitter << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "x" << YAML::Value << pivotPoint.x;
	_emitter << YAML::Key << "y" << YAML::Value << pivotPoint.y;
	_emitter << YAML::EndMap;


	std::string ttt = _emitter.c_str();
}
