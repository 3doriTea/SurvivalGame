#include "EnvConstant.h"
#include "MaterialRegistry.h"
#include "Graphic/MainCamera.h"
#include "Direct3D.h"
#include <Component/Camera.h>


GameBase::System::EnvConstant::EnvConstant() :
	hEnvMaterial_{ INVALID_HANDLE },
	constant_{}
{}

GameBase::System::EnvConstant::~EnvConstant()
{}

void GameBase::System::EnvConstant::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>*_registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<MaterialRegistry>())
		->Add(SystemRegistry::GetSystemIndex<MainCamera>())
		->Add(SystemRegistry::GetSystemIndex<Direct3D>())
	;
}

void GameBase::System::EnvConstant::UpdateSendTo(
	const UINT _slotNumber,
	const Mat4x4& _transformMatrix,
	EntityRegistry& _registry)
{
	constant_.transformMatrix = _transformMatrix;
	#pragma region TODO: カメラの定数は分けるべき
	Entity mainCamera{ Get<MainCamera>().GetEntity() };
	auto view{ _registry.GetView<Component::Camera>() };
	auto [camera]{ view.At(mainCamera) };
	constant_.viewProjectionMatrix = camera.GetViewProjectionMatrix();

	#pragma endregion

	// TODO: マテリアルから送信できるようにすべき
	Get<Direct3D>().Ref([this, _slotNumber](const ComPtr<ID3D11DeviceContext> _pContext)
		{
			const Material& material{ Get<MaterialRegistry>().At(hEnvMaterial_) };

			// コンスタントバッファのセット
			_pContext.Get()->VSSetConstantBuffers(_slotNumber, 1, material.pConstantBuffer.GetAddressOf());
			_pContext.Get()->PSSetConstantBuffers(_slotNumber, 1, material.pConstantBuffer.GetAddressOf());

			// コンスタントバッファの送信
			HRESULT hResult{};
			D3D11_MAPPED_SUBRESOURCE pData{ nullptr };
			if (hResult = _pContext.Get()->Map(
				material.pConstantBuffer.Get(),
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&pData);
				SUCCEEDED(hResult))
			{
				memcpy_s(
					pData.pData,
					pData.RowPitch,
					material.base.GetPtr(),
					material.base.Size());
				_pContext.Get()->Unmap(material.pConstantBuffer.Get(), 0);
			}
		});
}

void GameBase::System::EnvConstant::Initialize()
{
	// 自分自身がマテリアルベースを継承している
	hEnvMaterial_ = Get<MaterialRegistry>().Load(this);
	GB_ASSERT(hEnvMaterial_ && "環境定数マテリアルの読み込みに失敗");
}

void GameBase::System::EnvConstant::Update(EntityRegistry&)
{}

void GameBase::System::EnvConstant::Release()
{}
