#pragma once
#include "../SystemBase.h"
#include "Structure/RenderSurface.h"


namespace GameBase::System
{
	enum struct PresentSyncInterval : UINT
	{
		FULL,     // FPSを限界まで出す
		DEFAULT,  // 標準
		FIXED,    // 省電力 固定フレームレート
	};

	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	struct IPresenter : ISystemInterfaceBase
	{
		IPresenter() = default;
		virtual ~IPresenter() = default;

		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;

		virtual void Ref(const std::function<void(const ComPtr<ID3D11RenderTargetView>&)> _callback) = 0;
		/// <summary>
		/// レンダーターゲットを再登録
		/// </summary>
		virtual void RestoreMainRenderTarget() = 0;
		/// <summary>
		/// レンダーターゲットを解除
		/// </summary>
		virtual void ReleaseRenderTarget() = 0;
		/// <summary>
		/// シェーダリソースの掃除
		/// </summary>
		virtual void CleanShaderResource() = 0;

		/// <summary>
		/// 描画対象を設定する
		/// </summary>
		/// <param name="_p"></param>
		virtual void SetRenderSurface(RenderSurface* _p) = 0;

		/// <summary>
		/// 描画画面を作成する
		/// </summary>
		/// <param name="_pRenderSurface">作成された描画画面のポインタ渡し</param>
		/// <returns>作成された描画画面</returns>
		virtual bool TryCreateRenderSurface(RenderSurface* _pRenderSurface) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class Presenter : public SystemBase<Presenter, IPresenter>
	{
	public:
		Presenter();
		~Presenter();

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

		void BeginDraw() override;
		void EndDraw() override;
		void Ref(const std::function<void(const ComPtr<ID3D11RenderTargetView>&)> _callback) override;
		void RestoreMainRenderTarget() override;
		void ReleaseRenderTarget() override;
		void CleanShaderResource() override;

		/// <summary>
		/// 描画画面を作成する
		/// </summary>
		/// <param name="_pRenderSurface">作成された描画画面のポインタ渡し</param>
		/// <returns>作成された描画画面</returns>
		bool TryCreateRenderSurface(RenderSurface* _pRenderSurface) override;
		/// <summary>
		/// 描画する描画画面をセットする
		/// </summary>
		/// <param name="renderSurface"></param>
		void SetRenderSurface(RenderSurface* _renderSurface);

	private:
		ComPtr<IDXGIDevice> pDXGIDevice_;    // DXGIのデバイス
		ComPtr<IDXGIAdapter> pDXGIAdapter_;  // DXGIのアダプター
		ComPtr<IDXGIFactory> pDXGIFactory_;  // DXGIのファクトリー

		FLOAT clearDepthValue_;    // 深度バッファをクリアするときの値
		UINT clearStencilValue_;   // 深度バッファをクリアするときの値

		Color backgroundColor_;  // 背景色

		PresentSyncInterval syncInterval_;  // present関数の引数にわたす値 VSyncなどの設定

		RenderSurface* pRenderSurface_;   // 描画対象
	};
}
