#pragma once
#include <GameBase.h>


struct PlayerControllerData
{
	float moveSpeed = 30.0f;
};

struct PlayerController : ScriptBase<PlayerController, PlayerControllerData>
{
	void Start() override;
	void Update() override;
};
