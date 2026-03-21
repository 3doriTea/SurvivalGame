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

		virtual ModelHandle Load(const fs::path& _file) = 0;
		//virtual void Hoge() = 0;
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

	private:
		// モデル保管
		FlyweightFactory<Model, ModelHandle> models_;
	};
}
