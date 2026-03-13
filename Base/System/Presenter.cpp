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

#pragma region スワップチェーンの作成
			hResult = _pDevice.Get()->QueryInterface(
				__uuidof(IDXGIDevice),
				reinterpret_cast<void**>(pDXGIDevice_.GetAddressOf()));
			assert(SUCCEEDED(hResult) && "DXGIDeviceの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
#pragma endregion
#pragma region アダプターとか作成
			hResult = pDXGIDevice_.Get()->GetAdapter(pDXGIAdapter_.GetAddressOf());
			assert(SUCCEEDED(hResult) && "DXGIAdapterの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}

			hResult = pDXGIAdapter_.Get()->GetParent(
				__uuidof(IDXGIFactory),
				reinterpret_cast<void**>(pDXGIFactory_.GetAddressOf()));
			assert(SUCCEEDED(hResult) && "DXGIFactoryの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}


			GameConfig gameConfig{ Get<Assets>().GetGameConfig() };
			RefreshRate refreshRate{ gameConfig.fps };

			const Vec2Int SCREEN_SIZE{ gameConfig.windowSize };
			const bool WINDOWED{ gameConfig.Windowed };
			
			const DXGI_SWAP_CHAIN_DESC SWAP_CHAIN_DESC
			{
				.BufferDesc
				{
					.Width = static_cast<UINT>(SCREEN_SIZE.x),   // 描画領域の横幅
					.Height = static_cast<UINT>(SCREEN_SIZE.y),  // 描画領域の高さ
					.RefreshRate  // 画面を更新する間隔 (秒)
					{
						.Numerator = refreshRate.numerator,      // リフレッシュレートの分母
						.Denominator = refreshRate.denominator,  // リフレッシュレートの分子
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
				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,  // バックバッファの使い道は画面に描画すること
				.BufferCount = 1,  // バッファの枚数
				.OutputWindow = Get<MainWindow>().GetHandle(),
				.Windowed = WINDOWED,
				.SwapEffect = {},
				.Flags = NULL,
			};

			hResult = pDXGIFactory_->CreateSwapChain(
				_pDevice.Get(),
				const_cast<DXGI_SWAP_CHAIN_DESC*>(&SWAP_CHAIN_DESC),
				pSwapChain_.GetAddressOf());
			assert(SUCCEEDED(hResult) && "SwapChainの作成に失敗");
			if (failed = FAILED(hResult))
			{
				return;
			}
#pragma endregion
		});
}

void GameBase::System::Presenter::Update()
{
}

void GameBase::System::Presenter::Release()
{
}

void GameBase::System::Presenter::BeginDraw()
{
}

void GameBase::System::Presenter::EndDraw()
{
}

void GameBase::System::Presenter::Ref(const std::function<void(const ComPtr<ID3D11RenderTargetView>&)> _callback)
{
}
