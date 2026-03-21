#include "GameObject.h"

GameBase::Component::GameObject::GameObject() :
	name{}
{
}

void GameBase::Component::GameObject::OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle)
{
	std::string nameStr{ _node["name"].as<std::string>() };

	if (nameStr.size() >= GAME_OBJECT_NAME_BUFFER_SIZE)
	{
		nameStr = nameStr.substr(0, GAME_OBJECT_NAME_BUFFER_SIZE - 1);
	}

	GB_ASSERT(nameStr.size() < GAME_OBJECT_NAME_BUFFER_SIZE
		&& "名前は20byte以上保存できない");

	name.fill('\0');
	std::copy(nameStr.begin(), nameStr.end(), name.begin());
}

void GameBase::Component::GameObject::OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle)
{
	_emitter << YAML::Key << "name" << YAML::Value << name.data();
}

void GameBase::Component::GameObject::SetName(const std::string_view _name)
{
	std::string nameStr{ _name };
	if (nameStr.size() >= GAME_OBJECT_NAME_BUFFER_SIZE)
	{
		nameStr = nameStr.substr(0, GAME_OBJECT_NAME_BUFFER_SIZE - 1);
	}
	name.fill('\0');
	std::copy(nameStr.begin(), nameStr.end(), name.begin());
}
