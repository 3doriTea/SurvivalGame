#include "Script.h"

GameBase::Component::Script::Script() :
	hScript{ INVALID_HANDLE }
{}

void GameBase::Component::Script::OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle)
{}

void GameBase::Component::Script::OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle)
{}
