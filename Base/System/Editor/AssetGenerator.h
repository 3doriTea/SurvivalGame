#pragma once
#include "../SystemBase.h"
#include "../../Structure/AssetType.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IAssetGenerator : public ISystemInterfaceBase
	{
	public:
		IAssetGenerator() = default;
		virtual ~IAssetGenerator() = default;

		virtual bool TryGenerate(const fs::path& _file, const AssetType _type) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class AssetGenerator : public SystemBase<AssetGenerator, IAssetGenerator>
	{
	public:
		AssetGenerator();
		~AssetGenerator();

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
		/// 試しにアセットを生成する
		/// </summary>
		/// <param name="_file">ファイルパス</param>
		/// <param name="_type">アセットの種類</param>
		/// <returns>生成成功 true / false</returns>
		bool TryGenerate(const fs::path& _file, const AssetType _type) override;
	private:
	};
}
