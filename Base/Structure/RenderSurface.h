#pragma once
#include "RefreshRate.h"


namespace GameBase
{
	/// <summary>
	/// 描画画面
	/// </summary>
	struct RenderSurface
	{
		ComPtr<IDXGISwapChain> pSwapChain;                 // 表裏ひっくり返すやつ
		ComPtr<ID3D11RenderTargetView> pRenderTargetView;  // 描画対象へアクセスするやつ
		ComPtr<ID3D11Texture2D> pDepthStencil;             // 深度バッファ
		ComPtr<ID3D11DepthStencilView> pDepthStencilView;  // 深度バッファへアクセスするやつ

		D3D11_VIEWPORT viewport;  // ビューポート
		Vec2Int size;             // サイズ
		RefreshRate refreshRate;  // 更新頻度
		bool windowed;            // ウィンドウモードであるか
		HWND hWnd;                // ウィンドウハンドル
	};
}

