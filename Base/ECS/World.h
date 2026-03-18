#pragma once
#include "Entity.h"
#include "EntityRegistry.h"

namespace GameBase
{
	class IWorld
	{
	public:
		inline IWorld() = default;
		inline virtual ~IWorld() = default;

		void Registry();
	};

	/// <summary>
	/// ワールドそのもの
	/// </summary>
	class World
	{
	public:
		World(const EntityVersion _version);
		~World() = default;

		/// <summary>
		/// シーンを読み込む
		/// </summary>
		/// <param name="_sceneFile">シーンファイル</param>
		/// <param name="_initializeSystem">システムを初期化する</param>
		/// <returns>読み込みに成功 true / false</returns>
		bool TryLoadScene(const fs::path& _sceneFile, const bool _initializeSystem = false);

		bool Update();

		bool Release();

		/// <summary>
		/// システムをリロードする
		/// </summary>
		bool TryReloadSystems();

	private:
		const EntityVersion VERSION_;                // エンティティに付与するバージョン情報
		std::vector<int> systemOrderIndices_;        // システムの順番インデックス
		std::unique_ptr<EntityRegistry> pRegistry_;  // データが入るやつ
	};
}
