#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: DirectX11を扱うシステム
	/// </summary>
	struct IDirect3D : ISystemInterfaceBase
	{
		IDirect3D() = default;
		virtual ~IDirect3D() = default;

		virtual void Ref(const std::function<void(const ComPtr<ID3D11Device>&)>&) = 0;
		virtual void Ref(const std::function<void(const ComPtr<ID3D11DeviceContext>&)>&) = 0;
		virtual void Ref(const std::function<void(const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&)>&) = 0;
	};

	/// <summary>
	/// システム: DirectX11を扱う
	/// </summary>
	class Direct3D : public SystemBase<Direct3D, IDirect3D>
	{
	public:
		Direct3D();
		~Direct3D();

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

	public:
		void Ref(const std::function<void(const ComPtr<ID3D11Device>&)>& _callback) override;
		void Ref(const std::function<void(const ComPtr<ID3D11DeviceContext>&)>& _callback) override;
		void Ref(const std::function<void(const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&)>& _callback) override;

	private:
		ComPtr<ID3D11Device> pDevice_;
		ComPtr<ID3D11DeviceContext> pContext_;
	};
}
