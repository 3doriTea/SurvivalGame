#pragma once
#include "../MaterialBase.h"


namespace GameBase
{
	/// <summary>
	/// ポリゴンに対する情報 ラスタライザの処理後に使う情報
	/// </summary>
	struct Material
	{
		Material(const MaterialBase& _base, ComPtr<ID3D11Buffer>&& _p) :
			base{ _base },
			pConstantBuffer{ std::move(_p) }
		{}

		const MaterialBase& base;              // マテリアル
		ComPtr<ID3D11Buffer> pConstantBuffer;  // コンスタントバッファ
	};
}
