#include "TextureRegistry.h"
#include "Direct3D.h"


GameBase::System::TextureRegistry::TextureRegistry()
{}

GameBase::System::TextureRegistry::~TextureRegistry()
{}

void GameBase::System::TextureRegistry::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<Direct3D>())
	;
}

void GameBase::System::TextureRegistry::Initialize()
{
	Texture texture{};
	TryLoadTexture("./Assets/Default/InvalidTexture.png", {}, &texture);
	registry_.SetDefaultResource(std::move(texture));
}

void GameBase::System::TextureRegistry::Update()
{}

void GameBase::System::TextureRegistry::Release()
{}

GameBase::TextureHandle GameBase::System::TextureRegistry::Load(
	const fs::path& _file,
	const TextureLoadConfig& _loadConfig)
{

	TextureHandle found
	{
		registry_.Find([_file](const Texture& _at)
		{
			return _at.imageFile == _file;
		})
	};

	if (found != INVALID_HANDLE)
	{
		return found;
	}

	Texture texture{};
	if (TryLoadTexture(_file, _loadConfig, &texture) == false)
	{
		return INVALID_HANDLE;
	}
	

	return registry_.Emplace(std::move(texture));
}

GameBase::Vec2Int GameBase::System::TextureRegistry::GetImageSize(const TextureHandle _hTexture) const
{
	return registry_.At(_hTexture).imageSize;
}

void GameBase::System::TextureRegistry::RefAt(const TextureHandle _hTexture, std::function<void(const Texture&)> _callback) const
{
	_callback(registry_.At(_hTexture));
}

bool GameBase::System::TextureRegistry::TryLoadTexture(
	const fs::path& _file,
	const TextureLoadConfig& _loadConfig,
	GameBase::Texture* _pOut)
{
	using namespace DirectX;

	// 画像メタデータ
	TexMetadata metadata{};

	// 画像本体
	ScratchImage image{};

	HRESULT hResult{};

	hResult = LoadFromWICFile(
		_file.wstring().c_str(),
		WIC_FLAGS::WIC_FLAGS_NONE,
		&metadata,
		image);
	GB_ASSERT(SUCCEEDED(hResult) && "画像の読み込みに失敗");
	if (FAILED(hResult))
	{
		return false;
	}

	Texture texture{};

	texture.imageSize = { metadata.width, metadata.height };

	const D3D11_SAMPLER_DESC SAMPLER_DESC
	{
		.Filter = _loadConfig.filter,
		.AddressU = _loadConfig.addressU,
		.AddressV = _loadConfig.addressV,
		.AddressW = _loadConfig.addressW,
		.MipLODBias = _loadConfig.mipLODLevel,
	};

	Get<Direct3D>().Ref([&hResult, &texture, SAMPLER_DESC](const ComPtr<ID3D11Device>& _pDevice)
		{
			hResult = _pDevice.Get()->CreateSamplerState(
				&SAMPLER_DESC,
				texture.pSampler.GetAddressOf());
		});
	GB_ASSERT(SUCCEEDED(hResult) && "サンプラーの作成に失敗");
	if (FAILED(hResult))
	{
		return false;
	}

	const D3D11_SHADER_RESOURCE_VIEW_DESC SRV_DESC
	{
		.Format = _loadConfig.format,
		.ViewDimension = _loadConfig.viewDimension,
		.Buffer = _loadConfig.buffer.Buffer,
	};

	Get<Direct3D>().Ref([&hResult, &texture, &image, metadata, SRV_DESC](const ComPtr<ID3D11Device>& _pDevice)
		{
			hResult = CreateShaderResourceView(
				_pDevice.Get(),
				image.GetImages(),
				image.GetImageCount(),
				metadata,
				texture.pShaderResourceView.GetAddressOf());
		});
	GB_ASSERT(SUCCEEDED(hResult) && "シェーダリソースビュの作成に失敗");
	if (FAILED(hResult))
	{
		return false;
	}

	*_pOut = std::move(texture);

	return true;
}
