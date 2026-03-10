#pragma once
#include "Material.h"
#include "Mesh.h"

namespace GameBase
{
	/// <summary>
	/// 描画項目のソートに使うキー
	/// </summary>
	union SortKey
	{
		enum Layer
		{
			LAYER_BACKGROUND,   // 最も奥
			LAYER_OPAQUE,       // 不透明
			LAYER_TRANSPARENT,  // 半透明
			LAYER_UI,           // 手前のUI
		};

		uint64_t value;  // キーとしての値
		struct
		{
			uint64_t depth : 32;       // 固有の奥行き
			uint64_t materialId : 30;  // 使用するマテリアル
			uint64_t layer : 2;        // 大まかな括り
		} fields;                      // 詳細なビット
	};

	// ソートキーのサイズは8byteである
	static_assert(sizeof(SortKey) == 8);

	/// <summary>
	/// 描画項目
	/// </summary>
	struct RenderItem final
	{
		inline RenderItem(
			Mesh* _pMesh,
			Material* _pMaterial,
			const Mat4x4& _transformMatrix,
			const SortKey _sortKey) :
			pMesh{ _pMesh },
			pMaterial{ _pMaterial },
			transformMatrix{ _transformMatrix },
			sortKey{ _sortKey }
		{
			assert(pMesh && "pMesh must not nullptr");
			assert(pMaterial && "pMaterial must not nullptr");
		}

		Mesh* pMesh;          // not null メッシュのポインタ
		Material* pMaterial;  // not null マテリアルのポインタ

		Mat4x4 transformMatrix;  // 座標変換餃列

		SortKey sortKey;  // ソート時に使うキー
	};
}
