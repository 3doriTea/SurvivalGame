#pragma once
#include "Material.h"
#include "Mesh.h"
#include "SortKey.h"


namespace GameBase
{
	/// <summary>
	/// 描画項目
	/// </summary>
	struct RenderItem final
	{
		inline RenderItem(
			const Mesh* _pMesh,
			const Material* _pMaterial,
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

		const Mesh* pMesh;          // not null メッシュのポインタ
		const Material* pMaterial;  // not null マテリアルのポインタ
		Mat4x4 transformMatrix;     // 座標変換餃列

		SortKey sortKey;  // ソート時に使うキー
	};
}
