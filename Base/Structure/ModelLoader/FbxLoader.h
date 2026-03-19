#pragma once
#include <Library/ufbx/ufbx.h>
#include "../Model.h"
#include "FbxVertex.h"


namespace GameBase
{
	class FbxLoader
	{
	public:
		FbxLoader(const fs::path& _modelFile);
		inline FbxLoader() = default;

		bool TryLoad(Model* _pOut);

	private:
		MeshHandle LoadMesh(ufbx_mesh* _pMesh, ufbx_mesh_part* _pPart) const;
		bool TryCreateVertexBuffer(ComPtr<ID3D11Buffer>& _p, const std::vector<FbxVertex> _vertices) const;
		bool TryCreateIndexBuffer(ComPtr<ID3D11Buffer>& _p, const std::vector<uint32_t> _indices) const;

	private:
		fs::path modelFile_;  // ファイルパス
	};
}
