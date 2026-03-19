#pragma once
#include "../SystemBase.h"
#include "../MaterialBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IMaterialRegistry : public ISystemInterfaceBase
	{
	public:
		IMaterialRegistry() = default;
		virtual ~IMaterialRegistry() = default;

		virtual MaterialHandle Load(std::unique_ptr<MaterialBase> ) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class MaterialRegistry : public SystemBase<MaterialRegistry, IMaterialRegistry>
	{
	public:
		MaterialRegistry();
		~MaterialRegistry();

		// 利用する参照があるときに使用
		//void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

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
	
		MaterialHandle Load(std::unique_ptr<MaterialBase>) override;

	private:
	};
}
