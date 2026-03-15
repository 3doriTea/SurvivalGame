#pragma once

namespace GameBase
{
	/// <summary>
	/// シェーダのラスタライザ設定
	/// </summary>
	struct RasterizerConfig
	{
		RasterizerConfig() :
			cullMode{ D3D11_CULL_BACK },
			fillMode{ D3D11_FILL_SOLID },
			frontCounterClockwise{ false }
		{}

		D3D11_CULL_MODE cullMode;     // 面消去モード (カリング)
		D3D11_FILL_MODE fillMode;     // 塗り潰しモード
		bool frontCounterClockwise;   // 面の表 is 頂点の反時計回りで表すか
	};
}
