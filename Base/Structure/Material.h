#pragma once

namespace GameBase
{
	/// <summary>
	/// ポリゴンに対する情報 ラスタライザの処理後に使う情報
	/// </summary>
	struct Material
	{
		ComPtr<ID3D11Buffer> pConstantBuffer;  // コンスタントバッファ
	};
}
