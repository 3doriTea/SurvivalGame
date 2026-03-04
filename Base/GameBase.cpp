#include "GameBase.h"
#include "SystemBase.h"

bool GameBase::Game::Start()
{
	for (auto p : SystemRegistry::pSystems_)
	{
		if (auto pSystem{ p.lock() })
		{
			pSystem->Initialize();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool GameBase::Game::Update()
{
	return false;
}

bool GameBase::Game::End()
{
	return false;
}
