#include "EntityRegistry.h"


GameBase::EntityRegistry::EntityRegistry(const EntityVersion _version) :
	VERSION_{ _version }
{
}

GameBase::Entity GameBase::EntityRegistry::CreateEntity()
{
	EntityIndex index{ entityCounter_++ };
	EntityVersion version{ VERSION_ };
	Entity created{ MakeEntity(index, version) };
	return created;
}

void GameBase::EntityRegistry::AddComponent(const Entity _entity, ComponentType _type)
{
}

void GameBase::EntityRegistry::RemoveComponent(const Entity _entity, ComponentType _type)
{
}
