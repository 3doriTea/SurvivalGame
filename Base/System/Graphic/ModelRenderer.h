#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IModelRenderer : public ISystemInterfaceBase
	{
	public:
		IModelRenderer() = default;
		virtual ~IModelRenderer() = default;

		//virtual void Hoge() = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class ModelRenderer : public SystemBase<ModelRenderer, IModelRenderer>
	{
	public:
		ModelRenderer();
		~ModelRenderer();

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
	};
}
