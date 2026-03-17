#pragma once

namespace GameBase
{
	struct Texture
	{
		ComPtr<ID3D11SamplerState> pSampler;                   // サンプラー
		ComPtr<ID3D11ShaderResourceView> pShaderResourceView;  // シェーダリソースビュー
		Vec2Int imageSize;                                     // 画像サイズ
		fs::path imageFile;                                    // ファイルのパス
	};
}
