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
			Mat4x4 _toParentTransform,
			Mat4x4 _toWorldTransform) :
			hMesh{ _hMesh },
			hMaterial{ _hMaterial },
			toParentTransform{ _toParentTransform },
			toWorldTransform{ _toWorldTransform }
		{}


		ModelNode() :
			ModelNode{ INVALID_HANDLE, INVALID_HANDLE, Mat4x4::Identity(), Mat4x4::Identity() }
		{}

		MeshHandle hMesh;
		MaterialHandle hMaterial;
		Mat4x4 toParentTransform;
		Mat4x4 toWorldTransform;
	};

	struct Model
	{
		std::string name;  // モデルのId
		// ノード
		std::vector<ModelNode> nodes;
		AABB localAABB;  // カリング判定用AABB
	};
}
