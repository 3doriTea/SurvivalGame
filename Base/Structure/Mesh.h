#pragma once

namespace GameBase
{
	/// <summary>
	/// 頂点の情報
	/// </summary>
	struct Mesh
	{
		uint32_t vertexStructSize;  // 頂点の構造体サイズ
		uint32_t indexCount;        // インデックス数
		ComPtr<ID3D11Buffer> pVertexBuffer;  // 頂点バッファ
		ComPtr<ID3D11Buffer> pIndexBuffer;   // インデクスバッファ
	};
}
