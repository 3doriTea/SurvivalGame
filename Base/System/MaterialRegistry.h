#pragma once
#include "../SystemBase.h"
#include "../MaterialBase.h"
#include "../Structure/Material.h"
#include "../../Assets/Default/InvalidMaterial.h"


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

		virtual MaterialHandle Load(MaterialBase*) = 0;
		virtual const Material& At(const MaterialHandle _hMaterial) = 0;
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
	
		MaterialHandle Load(MaterialBase*) override;
		inline const Material& At(const MaterialHandle _hMaterial) { return materials_.At(_hMaterial); }

	private:
		bool TryGenerateConstantBuffer(MaterialBase* _pMaterialBase, ComPtr<ID3D11Buffer>& _out);

	private:
		FlyweightFactory<Material, MaterialHandle> materials_;

		InvalidMaterial invalidMaterial_;  // TODO: ここに持つべきではない
	};
}
