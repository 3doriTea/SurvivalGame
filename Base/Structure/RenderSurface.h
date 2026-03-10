#pragma once

namespace GameBase
{
	/// <summary>
	/// 描画画面に関する情報
	/// </summary>
	struct RenderSurface
	{
		ComPtr<IDXGISwapChain> pSwapChain;                 // 表裏ひっくり返すやつ
		ComPtr<ID3D11RenderTargetView> pRenderTargetView;  // 描画対象へアクセスするやつ
		ComPtr<ID3D11Texture2D> pDepthStencil;             // 深度バッファ
		ComPtr<ID3D11DepthStencilView> pDepthStencilView;  // 深度バッファへアクセスするやつ

		D3D11_VIEWPORT viewport;  // ビューポート
		uint32_t width;           // 横幅
		uint32_t height;          // 高さ
	};
}

