#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: シーンセーバ
	/// </summary>
	class ISceneSaver : public ISystemInterfaceBase
	{
	public:
		ISceneSaver() = default;
		virtual ~ISceneSaver() = default;

		/// <summary>
		/// 試しにシーンをセーブする
		/// </summary>
		/// <returns>セーブに成功した true / false</returns>
		virtual bool TrySaveScene(EntityRegistry& _registry, const fs::path& _file) = 0;
	};

	/// <summary>
	/// システム: シーンを保存する
	/// </summary>
	class SceneSaver : public SystemBase<SceneSaver, ISceneSaver>
	{
	public:
		SceneSaver();
		~SceneSaver();

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

		bool TrySaveScene(EntityRegistry& _registry, const fs::path& _file) override;
	private:
	};
}
