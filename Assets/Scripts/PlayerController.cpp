#include "PlayerController.h"
#include <System/Input.h>
#include <System/GameTime.h>
#include <Component/Transform.h>

void PlayerController::Start()
{
}

void PlayerController::Update()
{
	const auto& GAME_TIME = Get<System::GameTime>();
	const auto& INPUT = Get<System::Input>();
	const float deltaTime = GAME_TIME.DeltaTime();
	auto& transform = Get<Component::Transform>();

	Vec3 direction
	{
		INPUT.IsPress(KeyCode::D) ? 1.0f : 0.0f +
		INPUT.IsPress(KeyCode::A) ? -1.0f : 0.0f,
		0.0f,
		INPUT.IsPress(KeyCode::W) ? 1.0f : 0.0f +
		INPUT.IsPress(KeyCode::S) ? -1.0f : 0.0f,
	};
	Vec3 normalizedDirection = direction.Normalize();
	Vec3 move = normalizedDirection * moveSpeed;

	transform.position += move * deltaTime;
}
