#pragma once
#include "FileIdSchema.h"
#include "GameComponentSchema.h"
#include "GameObjectSchema.h"
#include "GameSceneSchema.h"


namespace GameBase::Schema
{
	struct YamlSchema
	{
		std::vector<GameScene> gameScenes;
		std::vector<GameObject> gameObjects;
		std::vector<GameComponent> gameComponents;
	};
}
