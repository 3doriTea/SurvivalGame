#include "pch.h"
#include "Camera.h"

GameBase::Component::Camera::Camera() :
	nearZ{ 0.1f },
	farZ{ 300.0f },
	fovRadian{ DirectX::XM_PIDIV4 },
	aspectRatio{},
	viewMatrix{},
	projectionMatrix{}
{
}
