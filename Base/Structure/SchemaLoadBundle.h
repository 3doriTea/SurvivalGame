#pragma once
#include "Schema/FileIdSchema.h"
#include "../ECS/Entity.h"

namespace GameBase
{
	/// <summary>
	/// シリアライゼーション中の変換マップバンドル
	/// </summary>
	struct SchemaLoadBundle
	{
		SchemaLoadBundle() :
			fileIdToEntity{ { 0, INVALID_ENTITY } },
			entityToFileId{ { INVALID_ENTITY, 0 } }
		{
		}

		// コピーしちゃだめ！
		SchemaLoadBundle(const SchemaLoadBundle& _other) = delete;
		SchemaLoadBundle& operator=(const SchemaLoadBundle& _other) = delete;

		std::map<Schema::FileId, Entity> fileIdToEntity{};
		std::map<Entity, Schema::FileId> entityToFileId{};
	};
}
