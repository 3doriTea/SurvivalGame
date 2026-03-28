#include "MeshRegistry.h"

GameBase::System::MeshRegistry::MeshRegistry()
{}

GameBase::System::MeshRegistry::~MeshRegistry()
{}

void GameBase::System::MeshRegistry::Initialize()
{
	meshes_.SetDefaultResource(Mesh{});
}

void GameBase::System::MeshRegistry::Update(EntityRegistry&)
{}

void GameBase::System::MeshRegistry::Release()
{}

GameBase::MeshHandle GameBase::System::MeshRegistry::Load(Mesh&& _mesh)
{
	GB_ASSERT(_mesh.indexCount > 0 && u8"indexが0以下は描画の意味なし");
	GB_ASSERT(_mesh.vertexStructSize > 0 && u8"頂点構造体のサイズが0以下は描画の意味なし");
	if (_mesh.pVertexBuffer && _mesh.pIndexBuffer)
	{
		return meshes_.Emplace(std::move(_mesh));
	}
	else
	{
		return INVALID_HANDLE;
	}
}
