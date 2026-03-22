#pragma once
#include "../SystemBase.h"
#include "../Structure/Model.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IModelRegistry : public ISystemInterfaceBase
	{
	public:
		IModelRegistry() = default;
		virtual ~IModelRegistry() = default;

		/// <summary>
		/// モデうをロードする
		/// </summary>
		/// <param name="_file">ファイルパス</param>
		/// <returns>モデルハンドル</returns>
		virtual ModelHandle Load(const fs::path& _file) = 0;
		/// <summary>
		/// モデルハンドルからモデルを参照する
		/// </summary>
		/// <param name="_handle">モデルハンドル</param>
		/// <returns>モデルの参照</returns>
		virtual const Model& At(const ModelHandle _handle) = 0;
		/// <summary>
		/// ファイル名からモデルを参照する
		/// </summary>
		/// <param name="_fileName">ファイル名</param>
		/// <returns>モデルの参照</returns>
		virtual const Model& At(const std::string& _fileName) = 0;
		/// <summary>
		/// ファイル名からモデルハンドルを取得する
		/// </summary>
		/// <param name="_fileName">ファイル名</param>
		/// <returns>モデルハンドル</returns>
		virtual ModelHandle Find(const std::string& _fileName) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class ModelRegistry : public SystemBase<ModelRegistry, IModelRegistry>
	{
	public:
		ModelRegistry();
		~ModelRegistry();

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

		ModelHandle Load(const fs::path& _file) override;
		inline const Model& At(const ModelHandle _handle) override { return models_.At(_handle); }
		const Model& At(const std::string& _fileName) override;
		ModelHandle Find(const std::string& _fileName) override;

	private:
		// モデル保管
		FlyweightFactory<Model, ModelHandle> models_;
	};
}
