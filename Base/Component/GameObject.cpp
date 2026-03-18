#include "GameObject.h"

GameBase::Component::GameObject::GameObject() :
	name{}
{
}

void GameBase::Component::GameObject::OnLoad(const YAML::Node& _node, SchemaLoadBundle& _bundle)
{
	std::string nameStr{ _node["name"].as<std::string>() };

	GB_ASSERT(nameStr.size() < GAME_OBJECT_NAME_BUFFER_SIZE
		&& "名前は20byte以上保存できない");

	memcpy(name.data(), nameStr.data(), nameStr.size());
}

void GameBase::Component::GameObject::OnSave(YAML::Emitter& _emitter, SchemaLoadBundle& _bundle)
{
	std::u8string_view nameStr{ name.data() };
	_emitter << YAML::Key << "name" << YAML::Value << nameStr.data();
}
