#pragma once
#include "ECS/World.h"
#include "ECS/EntityRegistry.h"


extern void Main();

namespace GameBase
{
	class Game final
	{
	public:
		Game();
		~Game() = default;

		bool Start();
		bool Update();
		bool End();

		/// <summary>
		/// シーンを作る
		/// </summary>
		void MakeScene();
		/// <summary>
		/// シーンを読み込み込む
		/// </summary>
		/// <param name="_sceneFile">シーンファイルのパス</param>
		void MoveScene(const fs::path& _sceneFile);
		// TODO:
		//fs::path SaveScene();

	private:
		std::unique_ptr<World> pWorld_;  // 今動かしているワールド
		std::vector<std::type_info> systemsTypeinfo_;  // システムの型情報
		std::unique_ptr<EntityRegistry> registries_;  // データベース
		EntityVersion versionCounter_;  // バージョンカウンタ
	};
}

using namespace GameBase;
