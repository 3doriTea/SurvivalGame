#include "GameEvent.h"

namespace
{
	std::map<std::type_index, std::unique_ptr<GameBase::GameEventBase>> eventRegistry_;
}

std::map<std::type_index, std::unique_ptr<GameBase::GameEventBase>>& GameBase::GameEvent::GetEventRegistry()
{
	return eventRegistry_;
}
