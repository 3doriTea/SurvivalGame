#pragma once
#include "Entity.h"
#include "IComponentBase.h"

namespace GameBase
{
	class IComponentPool
	{
	public:
		inline IComponentPool() = default;
		inline virtual ~IComponentPool() = default;

		virtual IComponentBase& Assign(const EntityIndex _index) = 0;
		virtual void Remove(const EntityIndex _index) = 0;
		virtual IComponentBase& GetI(const EntityIndex _index) = 0;
	};
}
