#pragma once
#include "Entity.h"
#include "EntityRegistry.h"

namespace GameBase
{
	/// <summary>
	/// ワールドそのもの
	/// </summary>
	class World
	{
	public:
		World(const EntityVersion _version);
		~World() = default;

		bool TryLoadScene(const fs::path& _sceneFile);

		bool Update();

		bool Release();

	private:
		const EntityVersion VERSION_;                // エンティティに付与するバージョン情報
		std::vector<int> systemOrderIndices_;        // システムの順番インデックス
		std::unique_ptr<EntityRegistry> pRegistry_;  // データが入るやつ
	};
}
