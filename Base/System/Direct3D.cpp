#include "Direct3D.h"


GameBase::System::Direct3D::Direct3D() :
	pDevice_{},
	pContext_{}
{
}

GameBase::System::Direct3D::~Direct3D()
{
}

void GameBase::System::Direct3D::Initialize()
{
	D3D_FEATURE_LEVEL featureLevel{};
	HRESULT hResult{};

	hResult = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG
		| D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		pDevice_.GetAddressOf(),
		&featureLevel,
		pContext_.GetAddressOf());

	GB_ASSERT(SUCCEEDED(hResult) && "D3D11CreateDeviceに失敗");
}

void GameBase::System::Direct3D::Update()
{
}

void GameBase::System::Direct3D::Release()
{
	pDevice_.Reset();
	pContext_.Reset();
}

void GameBase::System::Direct3D::Ref(const std::function<void(const ComPtr<ID3D11Device>&)>& _callback)
{
	_callback(pDevice_);
}

void GameBase::System::Direct3D::Ref(const std::function<void(const ComPtr<ID3D11DeviceContext>&)>& _callback)
{
	_callback(pContext_);
}

void GameBase::System::Direct3D::Ref(const std::function<void(const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&)>& _callback)
{
	_callback(pDevice_, pContext_);
}
