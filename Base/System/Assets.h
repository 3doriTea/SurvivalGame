#pragma once
#include "../SystemBase.h"
#include "Structure/GameConfig.h"
#include "Structure/GameManifest.h"
#include <Library/nlohmann-json/json.hpp>


namespace GameBase::System
{
	enum struct AssetsType : int
	{
		Shader,
		ShaderInclude,
	};

	/// <summary>
	/// システムインタフェース: アセットシステム
	/// </summary>
	struct IAssets : ISystemInterfaceBase
	{
		IAssets() = default;
		virtual ~IAssets() = default;

		/// <summary>
		/// アセットフォルダのパスを設定 && ロードする
		/// </summary>
		/// <param name="_dir">パス</param>
		virtual void SetDirectoryAndLoad(const fs::path& _dir) = 0;
		/// <summary>
		/// アセットフォルダ内の情報を取得する
		/// </summary>
		virtual void Load() = 0;

		/// <summary>
		/// 指定したJSONファイルを読み込む
		/// </summary>
		/// <param name="_file">アセットフォルダからの相対パス</param>
		/// <returns>json構造体</returns>
		virtual json FetchJson(const fs::path& _file) = 0;

		/// <summary>
		/// ゲームの設定を取得する
		/// </summary>
		/// <returns>ゲーム設定</returns>
		virtual GameConfig GetGameConfig() = 0;
		/// <summary>
		/// ゲームのマニフェスト情報(不変情報)を取得する
		/// </summary>
		/// <returns>ゲームのマニフェスト</returns>
		virtual GameManifest GetGameManifest() = 0;

		/// <summary>
		/// アセットファイルのパスを参照する
		/// </summary>
		/// <param name="_callback">パスvectorの参照コールバックvoid(const std::vector<fs::path>&)</param>
		/// <param name="_type">アセットファイルの種類</param>
		virtual void Ref(const std::function<void(const std::vector<fs::path>&)>& _callback, const AssetsType _type) = 0;
	};

	/// <summary>
	/// システム: アセット
	/// </summary>
	class Assets : public SystemBase<Assets, IAssets>
	{
	public:
		Assets();
		~Assets();

		// 利用する参照があるときに使用
		//void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		void SetDirectoryAndLoad(const fs::path& _dir) override;
		GameConfig GetGameConfig() override { return gameConfig_; }
		GameManifest GetGameManifest() override { return gameManifest_; }
		void Load() override;
		json FetchJson(const fs::path& _file) override;
		void Ref(const std::function<void(const std::vector<fs::path>&)>& _callback, const AssetsType _type) override;

	private:
		fs::path directory_;

		std::map<AssetsType, std::vector<fs::path>> typeToFilesPath_;  // アセットの種類別全ファイルパス
		GameConfig gameConfig_;
		GameManifest gameManifest_;

	};
}
