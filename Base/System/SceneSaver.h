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

		/// <summary>
		/// 編集フラグを立てる
		/// </summary>
		virtual void SetDirty() = 0;

		/// <summary>
		/// セーブが必要かチェックする
		/// </summary>
		virtual bool CheckSave() = 0;
	};

	/// <summary>
	/// システム: シーンを保存する
	/// </summary>
	class SceneSaver : public SystemBase<SceneSaver, ISceneSaver>
	{
	public:
		SceneSaver();
		~SceneSaver();

		// ワールド実行情報を参照、このシステムを登録除外するかチェックする true を返すことで除外される
		bool OnVerifyShouldSkip(const RunInfo& _info) override;

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

		bool CheckSave() override;
		bool TrySaveScene(EntityRegistry& _registry, const fs::path& _file) override;
		void SetDirty() override;

	private:
		EntityRegistry* pDangerPtrEntityRegistry_;   // TODO: あとで消す
		bool needSaveScene_;                         // シーンをセーブする必要があるか
		bool hasUnsaved_;                            // 未保存のデータがあるか
		std::shared_ptr<void> pWindowDestroyEvnet_;  // ウィンドウ終了イベント
	};
}
