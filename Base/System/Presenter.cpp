#include "Presenter.h"
#include "Direct3D.h"
#include "Assets.h"
#include "MainWindow.h"
#include "Structure/RefreshRate.h"


GameBase::System::Presenter::Presenter()
{
}

GameBase::System::Presenter::~Presenter()
{
}

void GameBase::System::Presenter::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
	->Add(SystemRegistry::GetSystemIndex<Direct3D>())
	->Add(SystemRegistry::GetSystemIndex<Assets>())
	->Add(SystemRegistry::GetSystemIndex<MainWindow>())
	;
}

void GameBase::System::Presenter::Initialize()
{
	bool failed{ true };

	Get<Direct3D>().Ref(
		[this, &failed](const ComPtr<ID3D11Device>& _pDevice, const ComPtr<ID3D11DeviceContext>& _pContext)
		{
			HRESULT hResult{};

#pragma region DXGIデバイス作成
			hResult = _pDevice.Get()->QueryInterface(
				__uuidof(IDXGIDevice),
				reinterpret_cast<void**>(pDXGIDevice_.GetAddressOf()));
			GB_ASSERT(SUCCEEDED(hResult) && "DXGIDeviceの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
#pragma endregion

#pragma region アダプターとか作成
			hResult = pDXGIDevice_.Get()->GetAdapter(pDXGIAdapter_.GetAddressOf());
			GB_ASSERT(SUCCEEDED(hResult) && "DXGIAdapterの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}

			hResult = pDXGIAdapter_.Get()->GetParent(
				__uuidof(IDXGIFactory),
				reinterpret_cast<void**>(pDXGIFactory_.GetAddressOf()));
			GB_ASSERT(SUCCEEDED(hResult) && "DXGIFactoryの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
#pragma endregion

		});


	bool succeed{ TryCreateRenderSurface(&renderSurface_) };
	GB_ASSERT(succeed);

	SetRenderSurface(renderSurface_);

	clearDepthValue_ = 1.0f;
	clearStencilValue_ = 0U;
	backgroundColor_ = { 0, 1, 0, 1 };
	syncInterval_ = PresentSyncInterval::DEFAULT;
}

void GameBase::System::Presenter::Update(EntityRegistry&)
{
}

void GameBase::System::Presenter::Release()
{
}

void GameBase::System::Presenter::BeginDraw()
{
	Get<Direct3D>().Ref([this](const ComPtr<ID3D11DeviceContext> _pContext)
	{
		_pContext.Get()->ClearRenderTargetView(
			renderSurface_.pRenderTargetView.Get(),
			backgroundColor_.f);

		_pContext.Get()->ClearDepthStencilView(
			renderSurface_.pDepthStencilView.Get(),
			D3D11_CLEAR_DEPTH,
			clearDepthValue_,
			clearStencilValue_);
	});
}

void GameBase::System::Presenter::EndDraw()
{
	HRESULT hResult{};

	hResult = renderSurface_.pSwapChain.Get()->Present(static_cast<UINT>(syncInterval_), 0);

	GB_ASSERT(SUCCEEDED(hResult) && "スワップチェーンのスワップに失敗");
}

void GameBase::System::Presenter::Ref(const std::function<void(const ComPtr<ID3D11RenderTargetView>&)> _callback)
{
	_callback(renderSurface_.pRenderTargetView);
}

void GameBase::System::Presenter::RestoreMainRenderTarget()
{
	Get<Direct3D>().Ref([this](const ComPtr<ID3D11DeviceContext> _pContext)
	{
		ID3D11RenderTargetView* rtbs[]{ renderSurface_.pRenderTargetView.Get() };
		_pContext.Get()->OMSetRenderTargets(1, rtbs, renderSurface_.pDepthStencilView.Get());
		_pContext.Get()->RSSetViewports(1, &renderSurface_.viewport);
	});
}

void GameBase::System::Presenter::ReleaseRenderTarget()
{
	Get<Direct3D>().Ref([this](const ComPtr<ID3D11DeviceContext> _pContext)
		{
			_pContext.Get()->OMSetRenderTargets(0, nullptr, nullptr);
		});
}

bool GameBase::System::Presenter::TryCreateRenderSurface(RenderSurface* _pRenderSurface)
{
	bool failed{ true };
	RenderSurface renderSurface{};

#pragma region 基本設定
	GameConfig gameConfig{ Get<Assets>().GetGameConfig() };
	renderSurface.refreshRate = gameConfig.fps;
	renderSurface.size        = gameConfig.windowSize;
	renderSurface.windowed    = gameConfig.Windowed;
	renderSurface.hWnd = Get<MainWindow>().GetHandle();
#pragma endregion

#pragma region ビューポートの設定
	renderSurface.size = Get<MainWindow>().GetClientSize();
	renderSurface.viewport =
	{
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = static_cast<float>(renderSurface.size.x),
		.Height = static_cast<float>(renderSurface.size.y),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};
#pragma endregion
	


	Get<Direct3D>().Ref(
		[this, &failed, &renderSurface](const ComPtr<ID3D11Device>& _pDevice, const ComPtr<ID3D11DeviceContext>& _pContext)
		{
			HRESULT hResult{};
			ComPtr<ID3D11Texture2D> pBackBuffer{};  // 取り出し用バッファ

#pragma region スワップチェーンの作成
			const DXGI_SWAP_CHAIN_DESC SWAP_CHAIN_DESC
			{
				.BufferDesc
				{
					.Width = static_cast<UINT>(renderSurface.size.x),   // 描画領域の横幅
					.Height = static_cast<UINT>(renderSurface.size.y),  // 描画領域の高さ
					.RefreshRate  // 画面を更新する間隔 (秒)
					{
						.Numerator = renderSurface.refreshRate.numerator,      // リフレッシュレートの分母
						.Denominator = renderSurface.refreshRate.denominator,  // リフレッシュレートの分子
					},
					.Format = DXGI_FORMAT_R8G8B8A8_UNORM,  // 何色使えるか
					.ScanlineOrdering = {},
					.Scaling = {}
				},
				.SampleDesc
				{
					.Count = 1,    // MSAA (アンチエイリアス) の設定
					.Quality = 0,  // 
				},
				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT,  // バックバッファの使い道は画面に描画することと、SRVも使うこと
				.BufferCount = 1,  // バッファの枚数
				.OutputWindow = renderSurface.hWnd,
				.Windowed = renderSurface.windowed,
				.SwapEffect = {},
				.Flags = NULL,
			};

			hResult = pDXGIFactory_->CreateSwapChain(
				_pDevice.Get(),
				const_cast<DXGI_SWAP_CHAIN_DESC*>(&SWAP_CHAIN_DESC),
				renderSurface.pSwapChain.GetAddressOf());
			GB_ASSERT(SUCCEEDED(hResult) && "SwapChainの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
#pragma endregion

#pragma region レンダーターゲットビューの作成
			hResult = renderSurface.pSwapChain->GetBuffer(
				0,
				__uuidof(ID3D11Texture2D),
				reinterpret_cast<void**>(pBackBuffer.GetAddressOf()));
			GB_ASSERT(SUCCEEDED(hResult) && "レンダーターゲットビュー用の一時バッファ取得に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}

			// 作成
			hResult = _pDevice.Get()->CreateRenderTargetView(
				pBackBuffer.Get(),
				nullptr,
				renderSurface.pRenderTargetView.GetAddressOf());
			GB_ASSERT(SUCCEEDED(hResult) && "レンダーターゲットビューの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
			
			pBackBuffer.Reset();  // もう使わないため解放
#pragma endregion

#pragma region 深度バッファを作成
			const D3D11_TEXTURE2D_DESC DEPTH_DESC
			{
				.Width = static_cast<UINT>(renderSurface.size.x),   // 描画領域の横幅
				.Height = static_cast<UINT>(renderSurface.size.y),  // 描画領域の高さ
				.MipLevels = 1,
				.ArraySize = 1,
				.Format = DXGI_FORMAT_D32_FLOAT,
				.SampleDesc
				{
					.Count = 1,
					.Quality = 0,
				},
				.Usage = D3D11_USAGE_DEFAULT,
				.BindFlags = D3D11_BIND_DEPTH_STENCIL,
				.CPUAccessFlags = 0,
				.MiscFlags = 0,
			};

			hResult = _pDevice.Get()->CreateTexture2D(
				&DEPTH_DESC,
				nullptr,
				renderSurface.pDepthStencil.GetAddressOf());
			GB_ASSERT(SUCCEEDED(hResult) && "レンダーターゲットビューの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
#pragma endregion

#pragma region 深度バッファビューを作成
			hResult = _pDevice.Get()->CreateDepthStencilView(
				renderSurface.pDepthStencil.Get(),
				nullptr,
				renderSurface.pDepthStencilView.GetAddressOf());
			GB_ASSERT(SUCCEEDED(hResult) && "レンダーターゲットビューの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
#pragma endregion

#pragma region シェーダリソースビューの作成
			// スワップチェーンからバックバッファを取り出しちゃう
			renderSurface.pSwapChain.Get()->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));

			_pDevice->CreateShaderResourceView(
				pBackBuffer.Get(),
				nullptr,
				renderSurface.pShaderResourceView.GetAddressOf());
			pBackBuffer.Reset();  // もう使わないため解放
#pragma endregion
		});

	if (failed)
	{
		// 失敗
		return false;
	}

	// 成功したので作成物を返す
	*_pRenderSurface = std::move(renderSurface);

	return true;
}

void GameBase::System::Presenter::SetRenderSurface(RenderSurface& _renderSurface)
{
	Get<Direct3D>().Ref([this, _renderSurface](const ComPtr<ID3D11DeviceContext> _pContext)
	{
		// MEMO: 3角形をどう定義するかの設定
		_pContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// MEMO: 描画先を1枚セット
		_pContext.Get()->OMSetRenderTargets(
			1,
			_renderSurface.pRenderTargetView.GetAddressOf(),
			_renderSurface.pDepthStencilView.Get());

		// MEMO: ビューポートを1枚セット
		_pContext.Get()->RSSetViewports(
			1,
			&_renderSurface.viewport);
	});
}
