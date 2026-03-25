#pragma once
#include "../SystemBase.h"
#include "../Structure/SchemaLoadBundle.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class ISchemaLinker : public ISystemInterfaceBase
	{
	public:
		ISchemaLinker() = default;
		virtual ~ISchemaLinker() = default;

		virtual void RefLoadBundle(const std::function<void(const SchemaLoadBundle&)>& _callback) const = 0;
		virtual const SchemaLoadBundle& GetSchemaLoadBundle() const = 0;
		virtual const std::map<ModelHandle, Schema::AssetId>& GetModelToAssetFile() const = 0;
		virtual const std::map<Schema::AssetId, ModelHandle>& GetAssetFileToModel() const = 0;
		/// <summary>
		/// スキーマリンクの再読み込み
		/// </summary>
		virtual void Reload() = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class SchemaLinker : public SystemBase<SchemaLinker, ISchemaLinker>
	{
	public:
		SchemaLinker();
		~SchemaLinker();

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
		/// 再読込する
		/// </summary>
		void Reload() override;

		void RefLoadBundle(const std::function<void(const SchemaLoadBundle&)>& _callback) const override;

		inline const SchemaLoadBundle& GetSchemaLoadBundle() const override { return loadBundle_; }
		const std::map<ModelHandle, Schema::AssetId>& GetModelToAssetFile() const override { return preModel2AssetFile_; }
		const std::map<Schema::AssetId, ModelHandle>& GetAssetFileToModel() const override { return preAssetFile2Model_; }

	private:
		SchemaLoadBundle loadBundle_;
		std::map<ModelHandle, Schema::AssetId> preModel2AssetFile_;  // プリロードしたモデルのファイルとモデルハンドル
		std::map<Schema::AssetId, ModelHandle> preAssetFile2Model_;  // プリロードしたモデルのファイルとモデルハンドル
		std::map<Schema::AssetId, ModelHandle> preAssetId2Model_;  // プリロードしたモデルハンドルとアセットId
	};
}
