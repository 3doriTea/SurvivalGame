#include "pch.h"
#include "Renderer.h"

GameBase::System::Renderer::Renderer()
{
}

GameBase::System::Renderer::~Renderer()
{
}

void GameBase::System::Renderer::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
}

void GameBase::System::Renderer::Initialize()
{
}

void GameBase::System::Renderer::Update()
{
}

void GameBase::System::Renderer::Release()
{
}

void GameBase::System::Renderer::OnBegin(const std::function<void(Event<>&)> _callback)
{
}

void GameBase::System::Renderer::OnEnd(const std::function<void(Event<>&)> _callback)
{
}
