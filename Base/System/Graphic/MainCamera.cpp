#include "MainCamera.h"
#include <Component/Transform.h>
#include <Component/Camera.h>
#include "../TransformCalculator.h"


GameBase::System::MainCamera::MainCamera() :
	cameraEntity_{ INVALID_ENTITY }
{}

GameBase::System::MainCamera::~MainCamera()
{}

void GameBase::System::MainCamera::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<TransformCalculator>())
	;
}

void GameBase::System::MainCamera::Initialize()
{}

void GameBase::System::MainCamera::Update(EntityRegistry& _registry)
{
	using namespace DirectX;

	auto view{ _registry.GetView<Component::Transform, Component::Camera>() };
	view.ForEach([this]
	(const Entity entity, Component::Transform& transform, Component::Camera& camera)
	{
		camera.projectionMatrix = XMMatrixPerspectiveFovLH(
			camera.fovRadian,
			camera.aspectRatio,
			camera.nearZ,
			camera.farZ);
		Vec3 worldPos{ transform.WorldPosition() };
		camera.viewMatrix = XMMatrixLookAtLH(worldPos, transform.Forward() + worldPos, transform.Up() + worldPos);

		cameraEntity_ = entity;
	});
}

void GameBase::System::MainCamera::Release()
{}
