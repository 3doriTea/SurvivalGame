#pragma once
#include "Collider/AABB.h"

namespace GameBase
{
	/// <summary>
	/// モデルのノード
	/// </summary>
	struct ModelNode
	{
		ModelNode(
			MeshHandle _hMesh,
			MaterialHandle _hMaterial,
			Mat4x4 _localTransform) :
			hMesh{ _hMesh },
			hMaterial{ _hMaterial },
			localTransform{ _localTransform }
		{}


		ModelNode() :
			ModelNode{ INVALID_HANDLE, INVALID_HANDLE, Mat4x4::Identity() }
		{}

		MeshHandle hMesh;
		MaterialHandle hMaterial;
		Mat4x4 localTransform;
	};

	struct Model
	{
		std::string name;  // モデルのId
		// メッシュとマテリアルのセット
		std::vector<ModelNode> meshAndMaterials;
		AABB localAABB;  // カリング判定用AABB
	};
}
