#include "ShaderCompiler.h"
#include "Assets.h"
#include "Direct3D.h"
#include <d3d11shader.h>


GameBase::System::ShaderCompiler::ShaderCompiler()
{}

GameBase::System::ShaderCompiler::~ShaderCompiler()
{}

void GameBase::System::ShaderCompiler::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>*_registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<Assets>())
		->Add(SystemRegistry::GetSystemIndex<Direct3D>())
	;
}

void GameBase::System::ShaderCompiler::Initialize()
{
	Get<Assets>().Ref([this](const std::vector<fs::path>& _filesPath)
	{
		shaders_.SetDefaultResource(Compile("./Assets/Default/InvalidShader.hlsl", {}, {}));

		for (const auto& filePath : _filesPath)
		{
			shaders_.Emplace(Compile(filePath, {}, {}));
		}
	}, AssetType_Shader);
}

void GameBase::System::ShaderCompiler::Update(EntityRegistry&)
{}

void GameBase::System::ShaderCompiler::Release()
{}

GameBase::Shader GameBase::System::ShaderCompiler::Compile(
	const fs::path& _file,
	const RasterizerConfig& _rasterizerConfig,
	const ShaderCompileConfig& _config)
{
	HRESULT hResult{};

	Shader shader{};

	// 頂点インプットレイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	ComPtr<ID3DBlob> pCompileError{};  // エラー格納用
	ComPtr<ID3DBlob> pCompileVS{};
	ComPtr<ID3DBlob> pCompilePS{};
#pragma region 頂点シェーダのコンパイル
	hResult = D3DCompileFromFile(
		_file.wstring().c_str(),
		nullptr,
		_config.pInclude,
		_config.vertexEntryName.c_str(),
		_config.vertexVersion.c_str(),
		_config.flags_,
		NULL,
		pCompileVS.GetAddressOf(),
		pCompileError.GetAddressOf());
	if (pCompileError)
	{
		GB_ASSERT(
			SUCCEEDED(hResult) && "頂点シェーダのコンパイルに失敗",
			static_cast<const char*>(pCompileError.Get()->GetBufferPointer()));
	}
	else
	{
		GB_ASSERT(SUCCEEDED(hResult) && "頂点シェーダのコンパイルに失敗");
	}
	if (FAILED(hResult))
	{
		return {};
	}
#pragma endregion

#pragma region ピクセルシェーダのコンパイル
	hResult = D3DCompileFromFile(
		_file.wstring().c_str(),
		nullptr,
		_config.pInclude,
		_config.pixelEntryName.c_str(),
		_config.pixelVersion.c_str(),
		_config.flags_,
		NULL,
		pCompilePS.GetAddressOf(),
		pCompileError.GetAddressOf());
	GB_ASSERT(
		SUCCEEDED(hResult) && "ピクセルシェーダのコンパイルに失敗",
		static_cast<const char*>(pCompileError.Get()->GetBufferPointer()));
	if (FAILED(hResult))
	{
		return {};
	}
#pragma endregion

#pragma region 頂点インプットレイアウトの抽出
	ComPtr<ID3D11ShaderReflection> pRefrection{};
	hResult = D3DReflect(
		pCompileVS.Get()->GetBufferPointer(),
		pCompileVS.Get()->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		reinterpret_cast<void**>(pRefrection.GetAddressOf()));
	GB_ASSERT(SUCCEEDED(hResult) && "頂点シェーダのリフレクションに失敗");
	if (FAILED(hResult))
	{
		return {};
	}

	D3D11_SHADER_DESC vsDesc{};
	hResult = pRefrection.Get()->GetDesc(&vsDesc);
	GB_ASSERT(SUCCEEDED(hResult) && "頂点シェーダのDESC取得に失敗");
	if (FAILED(hResult))
	{
		return {};
	}

	for (UINT i = 0; i < vsDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc{};
		pRefrection->GetInputParameterDesc(i, &paramDesc);
		GB_ASSERT(
			SUCCEEDED(hResult) && "頂点シェーダのDESC取得に失敗",
			std::format("Input Param Index:{}", i).c_str());
		if (FAILED(hResult))
		{
			return {};
		}

		D3D11_INPUT_ELEMENT_DESC desc{};
		desc.SemanticName = paramDesc.SemanticName;
		desc.SemanticIndex = paramDesc.SemanticIndex;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
		desc.InstanceDataStepRate = 0;

		// フォーマットをビットマスクから判定していくー
		switch (paramDesc.Mask)
		{
		case 0b1:
			switch (paramDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				desc.Format = DXGI_FORMAT_R32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				desc.Format = DXGI_FORMAT_R32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				desc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
			default:
				GB_ASSERT(
					false && "知らないコンポーネントフォーマット",
					std::format("component type:{}", static_cast<int>(paramDesc.ComponentType)).c_str());
				return {};
			}
			break;
		case 0b11:
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case 0b111:
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case 0b1111:
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		default:
			GB_ASSERT(
				false && "未対応のマスク",
				std::format("mask:{:b}", paramDesc.Mask).c_str());
			return {};
		}

		inputLayoutDesc.push_back(desc);
	}
#pragma endregion

	Get<Direct3D>().Ref([&hResult, &shader, _rasterizerConfig, pCompileVS, pCompilePS, inputLayoutDesc](const ComPtr<ID3D11Device> _pDevice)
	{
#pragma region 頂点シェーダの作成
		hResult = _pDevice.Get()->CreateVertexShader(
			pCompileVS.Get()->GetBufferPointer(),
			pCompileVS.Get()->GetBufferSize(),
			nullptr,
			shader.pVertexShader.GetAddressOf());
		GB_ASSERT(SUCCEEDED(hResult) && "頂点シェーダの作成に失敗");
		if (FAILED(hResult))
		{
			return;
		}
#pragma endregion

#pragma region ピクセルシェーダの作成
		hResult = _pDevice.Get()->CreatePixelShader(
			pCompilePS.Get()->GetBufferPointer(),
			pCompilePS.Get()->GetBufferSize(),
			nullptr,
			shader.pPixelShader.GetAddressOf());
		GB_ASSERT(SUCCEEDED(hResult) && "ピクセルシェーダの作成に失敗");
		if (FAILED(hResult))
		{
			return;
		}
#pragma endregion

#pragma region ラスタライザの作成
		const D3D11_RASTERIZER_DESC RASTERIZER_DESC
		{
			.FillMode = _rasterizerConfig.fillMode,
			.CullMode = _rasterizerConfig.cullMode,
			.FrontCounterClockwise = _rasterizerConfig.frontCounterClockwise,
		};

		hResult = _pDevice.Get()->CreateRasterizerState(
			&RASTERIZER_DESC,
			shader.pRasterizerState.GetAddressOf());
		GB_ASSERT(SUCCEEDED(hResult) && "ラスタライザステートの作成に失敗");
		if (FAILED(hResult))
		{
			return;
		}
#pragma endregion

#pragma region 頂点インプットレイアウトの作成
		hResult = _pDevice.Get()->CreateInputLayout(
			inputLayoutDesc.data(),
			static_cast<UINT>(inputLayoutDesc.size()),
			pCompileVS.Get()->GetBufferPointer(),
			pCompileVS.Get()->GetBufferSize(),
			shader.pVertexInputLayout.GetAddressOf());
		GB_ASSERT(SUCCEEDED(hResult) && "頂点インプットレイアウトの作成に失敗");
		if (FAILED(hResult))
		{
			return;
		}
#pragma endregion
	});

	return shader;
}

GameBase::Shader& GameBase::System::ShaderCompiler::At(const ShaderHandle _hShader)
{
	return shaders_.At(_hShader);
}

GameBase::ShaderHandle GameBase::System::ShaderCompiler::Load(const fs::path& _file, const RasterizerConfig& _rasterizerConfig, const ShaderCompileConfig& _config)
{
	return shaders_.Emplace(Compile(_file, _rasterizerConfig, _config));
}
