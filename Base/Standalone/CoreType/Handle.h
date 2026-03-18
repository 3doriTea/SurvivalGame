#pragma once

namespace GameBase
{
	/// <summary>
	/// 無効なハンドル
	/// </summary>
	static inline constexpr uint32_t INVALID_HANDLE{ 0 };

	/// <summary>
	/// シェーダハンドル
	/// </summary>
	using ShaderHandle = uint32_t;

	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	using TextureHandle = uint32_t;

	/// <summary>
	/// マテリアルハンドル
	/// </summary>
	using MaterialHandle = uint32_t;
}
