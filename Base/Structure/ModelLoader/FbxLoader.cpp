#include "FbxLoader.h"
#include <Library/ufbx/ufbx.h>
#include <System/Direct3D.h>
#include <System/MaterialRegistry.h>
#include <System/MeshRegistry.h>

#include "FbxMaterial.h"

namespace
{
	/// <summary>
	/// ufbx_matrixをMat4x4に変換
	/// </summary>
	/// <param name="_from">ufbx_matrix</param>
	/// <returns>Mat4x4</returns>
	static inline GameBase::Mat4x4 ConvertTOMat4x4(const ufbx_matrix& _from)
	{
		return GameBase::Mat4x4
		{
			_from.m00, _from.m01, _from.m02, _from.m03,
			_from.m10, _from.m11, _from.m12, _from.m13,
			_from.m20, _from.m21, _from.m22, _from.m23,
			      0.0,       0.0,       0.0,       1.0,
		};
	}
}


GameBase::FbxLoader::FbxLoader(const fs::path& _modelFile) :
	modelFile_{ _modelFile }
{}

bool GameBase::FbxLoader::TryLoad(Model* _pOut)
{
	ufbx_error error{};
	ufbx_load_opts options{};

	options.target_axes = ufbx_axes_left_handed_y_up;  // 左手座標系
	options.target_unit_meters = 1.0f;

	ufbx_scene* pScene{ ufbx_load_file(modelFile_.string().c_str(), &options, &error) };
	GB_ASSERT(pScene && "FBXシーンの読み込みに失敗",
		std::format("{}", error.description.data));
	if (!pScene)
	{
		return false;
	}

	Model model{};
	model.filePath = modelFile_;
	model.name = modelFile_.string();

	if (model.name != "N:\\workhome\\SurvivalGame\\Assets\\Models\\TestPlane\\TestPlane.fbx")
	{
		return false;
	}


	for (ufbx_node* pNode : pScene->nodes)
	{
		Debugger::LogF("{}", pNode->name.data);

		std::vector<uint32_t> usedMaterials{};

		//pScene->materials.
		// TODO: マテリアルと紐づける

		if (ufbx_mesh* pMesh{ pNode->mesh }; pMesh)
		{
			GB_ASSERT(pNode->materials.count == pMesh->material_parts.count);
			for (uint32_t i = 0; i < pMesh->material_parts.count; i++)
			{
				auto m =  pMesh->materials[0];
				//m->textures[0].texture->fi
				for (auto prop : m->props.props)
				{
					Debugger::LogF("prop.name = {}", prop.name.data);
				}
				Debugger::LogF(
					"i = {}, material{}",
					i,
					pNode->materials[i]->name.data);
				//pNode->mesh->face_material

				ufbx_mesh_part& part{ pMesh->material_parts[i] };

				MeshHandle hMesh{ LoadMesh(pNode->mesh, &part) };

				model.nodes.emplace_back(
					hMesh,
					INVALID_HANDLE,
					ConvertTOMat4x4(pNode->node_to_parent),
					ConvertTOMat4x4(pNode->node_to_world));
			}
		}

		//pNode->materials

		//pNode->node_to_parent
	}

	ufbx_free_scene(pScene);

	*_pOut = std::move(model);
	
	return true;  // TODO 後で成功にする
}

GameBase::MeshHandle GameBase::FbxLoader::LoadMesh(ufbx_mesh* _pMesh, ufbx_mesh_part* _pPart) const
{
	std::vector<FbxVertex> vertices{};
	std::vector<uint32_t> polyIndices;
	polyIndices.resize(_pMesh->max_face_triangles * 3);

	// 各マテリアルごとにポリゴンを処理
	for (uint32_t polyIndex : _pPart->face_indices)
	{
		ufbx_face poly{ _pMesh->faces[polyIndex] };

		// 面を三角化する
		uint32_t polyCount
		{
			ufbx_triangulate_face(
				polyIndices.data(), polyIndices.size(), _pMesh, poly)
		};

		// 三角形の各頂点を取得
		for (size_t i = 0; i < polyCount * 3; i++)
		{
			uint32_t index{ polyIndices[i] };

			FbxVertex v{};
			v.position = _pMesh->vertex_position[index].v;
			v.normal = _pMesh->vertex_normal[index].v;
			//ufbx_vec3 tangent{ _pMesh->vertex_tangent[index] };
			v.uv = _pMesh->vertex_uv[index].v;

			vertices.push_back(v);
		}
	}

	GB_ASSERT(vertices.size() == _pPart->num_triangles * 3
		&& "メッシュの三角化に失敗");

	// 頂点ストリーム
	ufbx_vertex_stream streams[]
	{
		{ vertices.data(), vertices.size(), sizeof(FbxVertex) },
	};

	std::vector<uint32_t> indices{};
	indices.resize(_pPart->num_triangles * 3);

	// 頂点重複を削除し、インデックスを作成
	size_t verticesCount
	{
		ufbx_generate_indices(streams, 1, indices.data(), indices.size(), nullptr, nullptr)
	};

	// 前詰めされたからリサイズする
	vertices.resize(verticesCount);

	Mesh mesh{};

	if (TryCreateVertexBuffer(mesh.pVertexBuffer, vertices)
		&& TryCreateIndexBuffer(mesh.pIndexBuffer, indices))
	{
		return Get<System::MeshRegistry>().Load(std::move(mesh));
	}

	return INVALID_HANDLE;
}

bool GameBase::FbxLoader::TryCreateVertexBuffer(ComPtr<ID3D11Buffer>& _p, const std::vector<FbxVertex> _vertices) const
{
	const D3D11_BUFFER_DESC DESC
	{
		.ByteWidth = static_cast<UINT>(sizeof(FbxVertex) * _vertices.size()),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		.MiscFlags = NULL,
		.StructureByteStride = 0,
	};

	D3D11_SUBRESOURCE_DATA data
	{
		.pSysMem = _vertices.data(),
	};

	HRESULT hResult{};
	Get<System::Direct3D>().Ref([&hResult, &_p, DESC, data](const ComPtr<ID3D11Device> _pDevice)
	{
		hResult = _pDevice.Get()->CreateBuffer(&DESC, &data, _p.GetAddressOf());
	});
	GB_ASSERT(SUCCEEDED(hResult) && "FbxLoadで頂点バッファの作成に失敗");
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

bool GameBase::FbxLoader::TryCreateIndexBuffer(ComPtr<ID3D11Buffer>& _p, const std::vector<uint32_t> _indices) const
{
	const D3D11_BUFFER_DESC DESC
	{
		.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * _indices.size()),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = NULL,
		.StructureByteStride = 0,
	};

	D3D11_SUBRESOURCE_DATA data
	{
		.pSysMem = _indices.data(),
	};

	HRESULT hResult{};
	Get<System::Direct3D>().Ref([&hResult, &_p, DESC, data](const ComPtr<ID3D11Device> _pDevice)
		{
			hResult = _pDevice.Get()->CreateBuffer(&DESC, &data, _p.GetAddressOf());
		});
	GB_ASSERT(SUCCEEDED(hResult) && "FbxLoadでインデックスバッファの作成に失敗");
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}
