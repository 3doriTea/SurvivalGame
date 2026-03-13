#include "Transform.h"


GameBase::Component::Transform::Transform():
	position{},
	rotation{},
	scale{},
	parent{}
{
}

void GameBase::Component::Transform::OnLoad(const YAML::Node& _node)
{}

void GameBase::Component::Transform::OnSave(YAML::Emitter & _emitter)
{}
