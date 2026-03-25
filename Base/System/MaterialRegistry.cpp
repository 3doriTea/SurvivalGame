#include "pch.h"
#include "MaterialRegistry.h"
#include "../System/Direct3D.h"


GameBase::System::MaterialRegistry::MaterialRegistry() :
	invalidMaterial_{}
{}

GameBase::System::MaterialRegistry::~MaterialRegistry()
{}

void GameBase::System::MaterialRegistry::Initialize()
{
	ComPtr<ID3D11Buffer> pBuffer{};
	bool succeed{ TryGenerateConstantBuffer(&invalidMaterial_, pBuffer) };
	GB_ASSERT(succeed && "無効マテリアルの作成に失敗");

	materials_.SetDefaultResource(*static_cast<const MaterialBase*>(&invalidMaterial_), std::move(pBuffer));
}

void GameBase::System::MaterialRegistry::Update(EntityRegistry&)
{}

void GameBase::System::MaterialRegistry::Release()
{}

GameBase::MaterialHandle GameBase::System::MaterialRegistry::Load(MaterialBase* _p)
{
	ComPtr<ID3D11Buffer> pBuffer{};

	if (TryGenerateConstantBuffer(_p, pBuffer))
	{
		return materials_.Emplace(*_p, std::move(pBuffer));
	}

	return INVALID_HANDLE;
}

bool GameBase::System::MaterialRegistry::TryGenerateConstantBuffer(MaterialBase* _pMaterialBase, ComPtr<ID3D11Buffer>& _out)
{
	HRESULT hResult{};
	Get<Direct3D>().Ref([&_out, &hResult, _pMaterialBase](const ComPtr<ID3D11Device> _pDevice)
		{
			GB_ASSERT(_pMaterialBase->Size() % 16 == 0 && "")
			const D3D11_BUFFER_DESC DESC
			{
				.ByteWidth = static_cast<UINT>(_pMaterialBase->Size()),
				.Usage = D3D11_USAGE_DYNAMIC,
				.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
				.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
				.MiscFlags = 0,
				.StructureByteStride = 0,
			};
			hResult = _pDevice.Get()->CreateBuffer(&DESC, nullptr, _out.GetAddressOf());
		});
	GB_ASSERT(SUCCEEDED(hResult) && "コンスタントバッファの作成に失敗");
	if (FAILED(hResult))
	{
		return false;
	}
	return true;
}

