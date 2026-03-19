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
	if (_mesh.pVertexBuffer && _mesh.pIndexBuffer)
	{
		return meshes_.Emplace(std::move(_mesh));
	}
	else
	{
		return INVALID_HANDLE;
	}
}
