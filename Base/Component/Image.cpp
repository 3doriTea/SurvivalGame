#include "Image.h"

GameBase::Component::Image::Image() :
	color{},
	hTexture{},
	hMaterial{}
{
}

void GameBase::Component::Image::OnLoad(const YAML::Node& _node, SchemaLoadBundle& _bundle)
{
	color.r = _node["color"]["r"].as<float>();
	color.g = _node["color"]["g"].as<float>();
	color.b = _node["color"]["b"].as<float>();
	color.a = _node["color"]["a"].as<float>();
}

void GameBase::Component::Image::OnSave(YAML::Emitter& _emitter, SchemaLoadBundle& _bundle)
{
	_emitter << YAML::Key << "color" << YAML::Value
		<< YAML::BeginMap
		<< YAML::Key << "r" << YAML::Value << color.r
		<< YAML::Key << "g" << YAML::Value << color.g
		<< YAML::Key << "b" << YAML::Value << color.b
		<< YAML::Key << "a" << YAML::Value << color.a
		<< YAML::EndMap;
}
