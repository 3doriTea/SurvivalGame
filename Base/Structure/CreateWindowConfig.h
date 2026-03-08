#pragma once
#include "RefreshRate.h"

namespace GameBase
{
	/// <summary>
	/// ウィンドウ作成時の設定
	/// </summary>
	struct CreateWindowConfig
	{
		std::string_view title;
		// REF: https://learn.microsoft.com/ja-jp/windows/win32/winmsg/window-class-styles
		UINT classStyle{ CS_VREDRAW | CS_HREDRAW };
		HICON icon;
		HICON iconSmile;
		HCURSOR cursor;
		// REF: https://learn.microsoft.com/ja-jp/windows/win32/winmsg/window-styles
		DWORD clientStyle{ WS_OVERLAPPEDWINDOW };  // クライアント領域のスタイル
		// REF: https://learn.microsoft.com/ja-jp/windows/win32/winmsg/extended-window-styles
		DWORD clientStyleEx{ WS_EX_OVERLAPPEDWINDOW };  // クライアント領域の拡張
		BOOL hasMenu{ FALSE };

		Vec2Int windowScreenSize;
		// ウィンドウの初期座標
		Vec2Int initPosition;
		// ウィンドウの親ウィンドウ
		HWND hWndParent{ nullptr };
		// 画面の更新頻度 (秒)
		RefreshRate refreshRateSec;
		// ウィンドウであるか
		BOOL windowed{ TRUE };
	};
}
