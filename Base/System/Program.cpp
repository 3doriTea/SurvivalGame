#include "Program.h"

GameBase::System::Program::Program() :
	hInstance_{ nullptr }
{
}

GameBase::System::Program::~Program()
{
}

void GameBase::System::Program::Initialize()
{
	hInstance_ = GetModuleHandle(NULL);
}

void GameBase::System::Program::Update(EntityRegistry&)
{
}

void GameBase::System::Program::Release()
{
}
