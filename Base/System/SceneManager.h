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

		//virtual void Hoge() = 0;
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
		void Update() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

	private:
		fs::path currentSceneFile_;  // 現在のシーンファイル
	};
}
