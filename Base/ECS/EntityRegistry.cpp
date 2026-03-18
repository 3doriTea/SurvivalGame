#include "EntityRegistry.h"


GameBase::EntityRegistry::EntityRegistry(const EntityVersion _version) :
	VERSION_{ _version },
	entityCounter_{}
{
}

GameBase::Entity GameBase::EntityRegistry::CreateEntity()
{
	EntityIndex index{ entityCounter_++ };
	EntityVersion version{ VERSION_ };
	Entity created{ MakeEntity(index, version) };

	entitySignatures_.push_back({});
	return created;
}

std::vector<GameBase::ComponentIndex> GameBase::EntityRegistry::GetComponentIndices(const Entity _entity)
{
	std::vector<ComponentIndex> indices{};

	Signature signature{ entitySignatures_[GetIndex(_entity)] };

	for (int i = 0; i < signature.size(); i++)
	{
		if (signature.test(i))
		{
			indices.push_back(i);
		}
	}

	return indices;
}

GameBase::IComponentBase& GameBase::EntityRegistry::AddComponent(const Entity _entity, const ComponentIndex _type)
{
	entitySignatures_[GetIndex(_entity)][_type] = true;
	return Get(_type).Assign(GetIndex(_entity));
}

void GameBase::EntityRegistry::RemoveComponent(const Entity _entity, const ComponentIndex _type)
{
	entitySignatures_[GetIndex(_entity)][_type] = false;
	Get(_type).Remove(GetIndex(_entity));
}

GameBase::IComponentBase& GameBase::EntityRegistry::GetComponent(
	const Entity _entity,
	const ComponentIndex _type)
{
	return Get(_type).GetI(GetIndex(_entity));
}
