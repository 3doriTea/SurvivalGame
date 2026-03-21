#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: シーンマネージャ
	/// </summary>
	class ISceneManager : public ISystemInterfaceBase
	{
	public:
		ISceneManager() = default;
		virtual ~ISceneManager() = default;

		/// <summary>
		/// 現在のシーン名を取得する
		/// </summary>
		/// <returns>シーン名</returns>
		virtual std::string GetCurrentName() = 0;

		/// <summary>
		/// 現在のシーンのファイルパスを取得
		/// </summary>
		/// <returns>現在のシーンのファイルパス</returns>
		virtual fs::path GetCurrentFile() = 0;

		/// <summary>
		/// <para>シーンファイルからシーン遷移する</para>
		/// <para>デフォルトシーンの場合emptyを返す</para>
		/// </summary>
		/// <param name="_file">シーンファイルのパス</param>
		virtual void LoadSceneFile(const fs::path& _file) = 0;
		/// <summary>
		/// <para>シーン名からシーン遷移する</para>
		/// </summary>
		/// <param name="_name">シーン名</param>
		virtual void LoadSceneName(const std::string& _name) = 0;
	};

	/// <summary>
	/// システム: シーンマネージャ
	/// </summary>
	class SceneManager : public SystemBase<SceneManager, ISceneManager>
	{
	public:
		SceneManager();
		~SceneManager();

		// 利用する参照があるときに使用
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update(EntityRegistry&) override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		/// <summary>
		/// 現在のシーン名を取得する
		/// </summary>
		/// <returns>現在のシーン名</returns>
		std::string GetCurrentName() override;

		void LoadSceneFile(const fs::path& _file) override;
		void LoadSceneName(const std::string& _name) override;
		inline fs::path GetCurrentFile() override { return currentSceneFile_; }

	private:
		/// <summary>
		/// シーン名 to ファイルパスを読み込む
		/// </summary>
		void PreloadNameToFile();

		/// <summary>
		/// シーンYAMLファイルからシーン名を切り取る
		/// </summary>
		/// <param name="_sceneYamlFile">シーンYAMLファイル</param>
		/// <returns>シーン名</returns>
		static std::string CutSceneName(const fs::path& _sceneYamlFile);

	private:
		std::map<std::string, fs::path> nameToFile_;  // シーン名 to ファイルパス
		fs::path currentSceneFile_;  // 現在のシーンファイル
	};
}
