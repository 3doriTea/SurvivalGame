#pragma once
#include "../../../ECS/EntityRegistry.h"
#include "../../SchemaLoadBundle.h"


namespace GameBase::Editor::Inspector
{
	class Component
	{
	public:
		Component(
			const Entity _entity,
			const ComponentIndex _componentIndex);
		inline ~Component() = default;

		bool IsUpdatedShow(EntityRegistry& _registry, std::string& _yamlStr, const SchemaLoadBundle& _loadBundle);

	public:  // 公開
		const Entity ENTITY;
		const ComponentIndex COMPONENT_INDEX;

	private:
		bool HasKeys(const YAML::Node& _node, const std::vector<std::string>& _keys) const;
	};
}
