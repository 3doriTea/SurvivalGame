#pragma once

namespace GameBase
{
	struct MaterialBase
	{
		MaterialBase() = default;
		virtual ~MaterialBase() = default;

		/// <summary>
		/// マテリアルのバイト数を取得する
		/// </summary>
		/// <returns>サイズ バイト数</returns>
		virtual size_t Size() const = 0;

		/// <summary>
		/// マテリアルに使用するシェーダ
		/// </summary>
		/// <returns>シェーダハンドル</returns>
		virtual ShaderHandle Shader() const = 0;

		/// <summary>
		/// マテリアルに使用するディフューズテクスチャ
		/// </summary>
		/// <returns>テクスチャハンドル</returns>
		inline virtual TextureHandle TextureDiffuse() const {}
		/// <summary>
		/// マテリアルに使用する法線マップ
		/// </summary>
		/// <returns>テクスチャハンドル</returns>
		inline virtual TextureHandle TextureNormal() const {}
	};
}
