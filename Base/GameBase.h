#pragma once
#include "ECS/World.h"
#include "ECS/EntityRegistry.h"
#include "Structure/RunInfo.h"


extern void Main();

namespace GameBase
{
	class Game final
	{
	public:
		Game(const RunMode _runMode);
		~Game() = default;

		bool Start(const fs::path& _startScenePath = "Assets/Default/Boot.scene.yaml");
		bool Update();
		bool End();

		/// <summary>
		/// シーンを作る
		/// </summary>
		void MakeScene(const RunInfo& _runInfo, const bool _reloadSystems = false);
		/// <summary>
		/// シーンを読み込み込む
		/// </summary>
		/// <param name="_sceneFile">シーンファイルのパス</param>
		void MoveScene(const fs::path& _sceneFile);
		// TODO:
		//fs::path SaveScene();

	private:
		RunMode runMode_;  // 実行モード
		std::unique_ptr<World> pWorld_;  // 今動かしているワールド
		std::vector<std::type_info> systemsTypeinfo_;  // システムの型情報
		std::unique_ptr<EntityRegistry> registries_;  // データベース
		EntityVersion versionCounter_;  // バージョンカウンタ
	};
}
// TODO: ゲーム側にリクエストを出す、リクエストを確認しシーン遷移やゲーム終了などを行うものをつくうる
using namespace GameBase;
