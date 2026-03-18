#include "pch.h"
#include "MaterialRegistry.h"

GameBase::System::MaterialRegistry::MaterialRegistry()
{}

GameBase::System::MaterialRegistry::~MaterialRegistry()
{}

void GameBase::System::MaterialRegistry::Initialize()
{}

void GameBase::System::MaterialRegistry::Update(EntityRegistry&)
{}

void GameBase::System::MaterialRegistry::Release()
{}

GameBase::MaterialHandle GameBase::System::MaterialRegistry::Load(std::unique_ptr<MaterialBase>)
{
	return MaterialHandle();
}
