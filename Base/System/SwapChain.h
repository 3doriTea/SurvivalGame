#pragma once
#include "../SystemBase.h"


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
	struct ISwapChain : ISystemInterfaceBase
	{
		ISwapChain() = default;
		virtual ~ISwapChain() = default;

		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;

		virtual void Ref(const std::function<void(const ComPtr<ID3D11RenderTargetView>&)> _callback) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class SwapChain : public SystemBase<SwapChain, ISwapChain>
	{
	public:
		SwapChain();
		~SwapChain();

		// 利用する参照があるときに使用
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		void BeginDraw() override;
		void EndDraw() override;
		void Ref(const std::function<void(const ComPtr<ID3D11RenderTargetView>&)> _callback) override;

	private:
		ComPtr<IDXGIDevice> pDXGIDevice_;    // DXGIのデバイス
		ComPtr<IDXGIAdapter> pDXGIAdapter_;  // DXGIのアダプター
		ComPtr<IDXGIFactory> pDXGIFactory_;  // DXGIのファクトリー

		ComPtr<IDXGISwapChain> pSwapChain_;  // スワップチェーン

		ComPtr<ID3D11RenderTargetView> pRenderTargetView_;  // レンダーターゲットビュー
		ComPtr<ID3D11Texture2D> pDepthBuffer_;              // 深度バッファ
		ComPtr<ID3D11DepthStencilView> pDepthStencilView_;  // 震度ステンシルビュー

		FLOAT clearDepthValue_;    // 深度バッファをクリアするときの値
		UINT clearStencilValue_;   // 深度バッファをクリアするときの値

		PresentSyncInterval syncInterval_;  // present関数の引数にわたす値 VSyncなどの設定
	};
}
