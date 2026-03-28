#pragma once
#include "../SystemBase.h"
#include "../../Structure/UI/UIMaterial.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IImageRenderer : public ISystemInterfaceBase
	{
	public:
		IImageRenderer() = default;
		virtual ~IImageRenderer() = default;

		//virtual void Hoge() = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class ImageRenderer : public SystemBase<ImageRenderer, IImageRenderer>
	{
	public:
		ImageRenderer();
		~ImageRenderer();

		// ワールド実行情報を参照、このシステムを登録除外するかチェックする true を返すことで除外される
		//bool OnVerifyShouldSkip(const RunInfo& _info) override;

		// 他システムへの依存関係を登録する cppでincludeした他システムはここで登録
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

	private:
		MeshHandle hMesh_;
		MaterialHandle hMaterial_;
		std::unique_ptr<UIMaterial> pMaterial_;
	};
}
