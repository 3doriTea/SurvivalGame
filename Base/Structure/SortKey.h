#pragma once


namespace GameBase
{
	constexpr uint32_t SORT_KEY_MATERIAL_ID_MAX{ (1ULL << 30) - 1 };  // 2^30 - 1

	/// <summary>
	/// 描画項目のソートに使うキー
	/// </summary>
	union SortKey
	{
		using Layer = uint64_t;
		enum Layer_
		{
			Layer_Background  = 0,  // 最も奥
			Layer_Opaque      = 1,  // 不透明
			Layer_Transparent = 2,  // 半透明
			Layer_UI          = 3,  // 手前のUI
		};

		uint64_t value;  // キーとしての値
		struct
		{
			uint64_t depth : 32;       // 固有の奥行き
			uint64_t materialId : 30;  // 使用するマテリアル
			Layer layer : 2;           // 大まかな括り
		} fields;                      // 詳細なビット

		/// <summary>
		/// ソートキーを作る
		/// </summary>
		/// <param name="_layer">レイヤー(4bit)</param>
		/// <param name="_materialId">マテリアルId(30bit)</param>
		/// <param name="_depth">小さければ先、大きければ手前,深度(32bit)</param>
		/// <returns></returns>
		inline static SortKey Make(
			const Layer _layer,
			const uint32_t _materialId,
			const uint32_t _depth = UINT32_MAX / 2)
		{
			GB_ASSERT(_materialId <= SORT_KEY_MATERIAL_ID_MAX
				&& "マテリアルIdは30bitで表せる範囲である必要があります");
			return SortKey
			{
				.fields
				{
					.depth = _depth,
					.materialId = _materialId,
					.layer = _layer,
				}
			};
		}
	};

	// ソートキーのサイズは8byteである
	static_assert(sizeof(SortKey) == 8);
}
