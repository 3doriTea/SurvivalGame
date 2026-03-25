#pragma once
#include "../SystemBase.h"
#include "../Structure/Mesh.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IMeshRegistry : public ISystemInterfaceBase
	{
	public:
		IMeshRegistry() = default;
		virtual ~IMeshRegistry() = default;

		/// <summary>
		/// メッシュをロードする
		/// </summary>
		/// <returns>メッシュハンドル</returns>
		virtual MeshHandle Load(Mesh&& _mesh) = 0;

		virtual const Mesh& At(const MeshHandle _hMesh) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class MeshRegistry : public SystemBase<MeshRegistry, IMeshRegistry>
	{
	public:
		MeshRegistry();
		~MeshRegistry();

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

		MeshHandle Load(Mesh&& _mesh) override;
		inline const Mesh& At(const MeshHandle _hMesh) { return meshes_.At(_hMesh); }

	private:
		FlyweightFactory<Mesh, MeshHandle> meshes_;
	};
}
