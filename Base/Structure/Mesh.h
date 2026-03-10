#pragma once

namespace GameBase
{
	/// <summary>
	/// 頂点の情報
	/// </summary>
	struct Mesh
	{
		ComPtr<ID3D11Buffer> pVertexBuffer;  // 頂点バッファ
		ComPtr<ID3D11Buffer> pIndexBuffer;   // インデクスバッファ
	};
}
