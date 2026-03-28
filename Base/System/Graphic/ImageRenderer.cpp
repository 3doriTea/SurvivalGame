#include "ImageRenderer.h"
#include <Component/Image.h>
#include <Component/Transform.h>
#include "../GameTime.h"
#include "../TransformCalculator.h"
#include "../MaterialRegistry.h"
#include "../MeshRegistry.h"
#include "../Renderer.h"
#include "../Direct3D.h"
#include "../ShaderCompiler.h"
#include "../TextureRegistry.h"

#include "../../Structure/UI/UIVertex.h"


GameBase::System::ImageRenderer::ImageRenderer() :
	hMesh_{},
	hMaterial_{},
	pMaterial_{}
{}

GameBase::System::ImageRenderer::~ImageRenderer()
{}

void GameBase::System::ImageRenderer::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>*_registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<TransformCalculator>())
		->Add(SystemRegistry::GetSystemIndex<MeshRegistry>())
		->Add(SystemRegistry::GetSystemIndex<MaterialRegistry>())
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		->Add(SystemRegistry::GetSystemIndex<Direct3D>())
		->Add(SystemRegistry::GetSystemIndex<ShaderCompiler>())
		->Add(SystemRegistry::GetSystemIndex<TextureRegistry>())
		;
}

void GameBase::System::ImageRenderer::Initialize()
{
	Mesh mesh{};

	{  // 頂点バッファの作成
		enum Vertex_
		{
			Vertex_TopLeft,
			Vertex_BottomLeft,
			Vertex_BottomRight,
			Vertex_TopRight,
			Vertex_Max,
		};
		std::array<UIVertex, Vertex_Max> vertices{};

		vertices[Vertex_TopLeft]     = { { -1,  1 }, { 0, 0 } };
		vertices[Vertex_BottomLeft]  = { { -1, -1 }, { 0, 1 } };
		vertices[Vertex_BottomRight] = { {  1, -1 }, { 1, 1 } };
		vertices[Vertex_TopRight]    = { {  1,  1 }, { 1, 0 } };

		const D3D11_BUFFER_DESC DESC
		{
			.ByteWidth = static_cast<UINT>(sizeof(UIVertex) * vertices.size()),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = NULL,
			.StructureByteStride = 0,
		};

		D3D11_SUBRESOURCE_DATA data
		{
			.pSysMem = vertices.data(),
		};

		HRESULT hResult{};
		Get<System::Direct3D>().Ref([&hResult, &mesh, DESC, data](const ComPtr<ID3D11Device> _pDevice)
			{
				hResult = _pDevice.Get()->CreateBuffer(&DESC, &data, mesh.pVertexBuffer.GetAddressOf());
			});
		GB_ASSERT(SUCCEEDED(hResult) && "ImageRenderer::Initで頂点バッファの作成に失敗");
	}

	{
		std::array<uint32_t, 6> indices{ 0, 1, 3, 3, 1, 2 };

		const D3D11_BUFFER_DESC DESC
		{
			.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indices.size()),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_INDEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = NULL,
			.StructureByteStride = 0,
		};

		D3D11_SUBRESOURCE_DATA data
		{
			.pSysMem = indices.data(),
		};

		HRESULT hResult{};
		Get<System::Direct3D>().Ref([&hResult, &mesh, DESC, data](const ComPtr<ID3D11Device> _pDevice)
			{
				hResult = _pDevice.Get()->CreateBuffer(&DESC, &data, mesh.pIndexBuffer.GetAddressOf());
			});
		GB_ASSERT(SUCCEEDED(hResult) && "ImageRenderer::Initでインデックスバッファの作成に失敗");

		mesh.indexCount = static_cast<UINT>(indices.size());
	}
	mesh.vertexStructSize = sizeof(UIVertex);

	hMesh_ = Get<MeshRegistry>().Load(std::move(mesh));

	RasterizerConfig rasterizerConfig{};
	rasterizerConfig.cullMode = D3D11_CULL_NONE;

	ShaderHandle hUIShader
	{
		Get<ShaderCompiler>().Load(
			"./Assets/Default/UI.hlsl",
			rasterizerConfig,
			{})
	};
	GB_ASSERT(hUIShader && "UIシェーダの読み込みに失敗");

	pMaterial_ = std::make_unique<UIMaterial>(hUIShader, UIMaterialConstant{ Color::WHITE });

	hMaterial_ = Get<MaterialRegistry>().Load(pMaterial_.get());
}

void GameBase::System::ImageRenderer::Update(EntityRegistry& _registry)
{
	using Component::Transform;
	using Component::Image;

	if (!Get<GameTime>().IsFrameDue())
	{
		return;  // 描画タイミング以外は無視
	}

	auto view{ _registry.GetView<Transform, Image>() };

	view.ForEach([this](const Entity _entity, Transform& _transform, Image& _image)
		{
			Get<Renderer>().Enqueue(RenderItem
				{
					&Get<MeshRegistry>().At(hMesh_),
					&Get<MaterialRegistry>().At(hMaterial_),
					_transform.GetWorldMatrix(),
					SortKey::Make(SortKey::Layer_UI, 0),
				});
		});
}

void GameBase::System::ImageRenderer::Release()
{}
