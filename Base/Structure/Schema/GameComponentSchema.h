#pragma once
#include "FileIdSchema.h"
#include "../../ComponentIndex.h"

namespace GameBase::Schema
{
	struct GameComponent
	{
		FileId self;
		ComponentIndex index;
		Entity entity;     // 所属するエンティティ
		std::string tag;   // !gbc!で始まる (GameObjectは例外)
		std::string name;  // コンポーネントの名前
		YAML::Node node;
	};
}
