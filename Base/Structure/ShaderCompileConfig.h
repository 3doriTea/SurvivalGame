#pragma once

namespace GameBase
{
	/// <summary>
	/// シェーダコンパイル時の設定
	/// </summary>
	struct ShaderCompileConfig
	{
		ShaderCompileConfig() :
			vertexEntryName{ "VS" },
			vertexVersion{ "vs_5_0" },
			pixelEntryName{ "PS" },
			pixelVersion{ "ps_5_0" },
			pInclude{ D3D_COMPILE_STANDARD_FILE_INCLUDE },
			flags_{ D3DCOMPILE_ENABLE_STRICTNESS }
		{}



		std::string vertexEntryName;  // 頂点シェーダエントリポイント名
		std::string vertexVersion;    // 頂点シェーダのコンパイラバージョン
		std::string pixelEntryName;   // ピクセルシェーダエントリポイント名
		std::string pixelVersion;     // ピクセルシェーダのコンパイラバージョン
		ID3DInclude* pInclude;        // インクルード設定
		UINT flags_;                  // 諸々のフラグ
	};
}
