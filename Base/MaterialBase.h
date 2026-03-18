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
		/// マテリアルに使用するテクスチャ
		/// </summary>
		/// <returns></returns>
		inline virtual TextureHandle Texture() const {};
	};
}
