#pragma once
#include "Entity.h"

namespace GameBase
{
	class IComponentPool
	{
	public:
		inline IComponentPool() = default;
		inline virtual ~IComponentPool() = default;

		virtual void Remove(const EntityIndex _index) = 0;
	};
}
