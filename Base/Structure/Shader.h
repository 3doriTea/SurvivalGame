#pragma once
#include "RasterizerConfig.h"

namespace GameBase
{
	struct Shader
	{
		std::string name;  // シェーダ名
		ComPtr<ID3D11VertexShader> pVertexShader;        // 頂点シェーダ
		ComPtr<ID3D11PixelShader> pPixelShader;          // ピクセルシェーダ
		ComPtr<ID3D11InputLayout> pVertexInputLayoyt;    // 頂点インプットレイアウト
		ComPtr<ID3D11RasterizerState> pRasterizerState;  // ラスタライザ
		RasterizerConfig rasterizerConfig;               // ラスタライザの設定
	};
}
