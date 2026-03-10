#pragma once

namespace GameBase
{
	/// <summary>
	/// ゲーム設定
	/// </summary>
	struct GameConfig
	{
		Vec2Int windowSize;     // ウィンドウサイズ
		Vec2Int windowInitPos;  // ウィンドウ起動座標
		uint32_t fps;                // フレームレート
		bool vSync;             // 垂直同期
		bool Windowed;          // ウィンドウモードか
	};
}
