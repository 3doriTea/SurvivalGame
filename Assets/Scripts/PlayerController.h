#pragma once
#include <ScriptBase.h>

using namespace GameBase;

struct PlayerControllerData
{
	Entity player;
};

struct PlayerController : ScriptBase<PlayerController, PlayerControllerData>
{
	void Start() override;
	void Update() override;
};
