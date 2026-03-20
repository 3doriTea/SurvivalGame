#include "View.h"
#include "EntityRegistry.h"

GameBase::ViewEntityOnly::ViewEntityOnly(EntityRegistry& _registry) :
	registry_{ _registry }
{}

void GameBase::ViewEntityOnly::ForEach(const std::function<void(Entity)>& _callback)
{
	for (EntityIndex index = 0; index < registry_.entityCounter_; index++)
	{
		_callback(MakeEntity(index, registry_.VERSION_));
	}
}
