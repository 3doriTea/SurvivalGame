#include "MainWindow.h"
#include "Assets.h"
#include "WindowEvent.h"
#include "Program.h"


GameBase::System::MainWindow::MainWindow() :
	clientSize_{ Vec2Int::Zero() },
	windowSize_{ Vec2Int::Zero() },
	handle_{ nullptr }
{
}

GameBase::System::MainWindow::~MainWindow()
{
}

void GameBase::System::MainWindow::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
	->Add(SystemRegistry::GetSystemIndex<Assets>())
	->Add(SystemRegistry::GetSystemIndex<WindowEvent>())
	->Add(SystemRegistry::GetSystemIndex<Program>())
	;
}

void GameBase::System::MainWindow::Initialize()
{
	GameManifest gameManifest{ Get<Assets>().GetGameManifest() };
	GameConfig gameConfig{ Get<Assets>().GetGameConfig() };
	RefreshRate refreshRate{ gameConfig.fps };

	Create(
		{
			.title = gameManifest.title,
			.classStyle = CS_VREDRAW | CS_HREDRAW,
			.icon = LoadIcon(nullptr, IDI_APPLICATION),
			.iconSmile = LoadIcon(nullptr, IDI_WINLOGO),
			.cursor = LoadCursor(nullptr, IDC_ARROW),
			.clientStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
			.clientStyleEx = WS_EX_OVERLAPPEDWINDOW,
			.hasMenu = FALSE,
			.windowScreenSize = gameConfig.windowSize,
			.initPosition = gameConfig.windowInitPos,
			.hWndParent = nullptr,
			.refreshRateSec = refreshRate,
		});
}

void GameBase::System::MainWindow::Update()
{
}

void GameBase::System::MainWindow::Release()
{
}

void GameBase::System::MainWindow::Create(const CreateWindowConfig& _config)
{
	HINSTANCE hInstance{ Get<Program>().GetInstance() };

	const WNDCLASSEX WNDCLASSEX_DESC
	{
		.cbSize = sizeof(WNDCLASSEX),
		.style = _config.classStyle,
		.lpfnWndProc = Get<WindowEvent>().GetWndProc(),
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = _config.icon,
		.hCursor = _config.cursor,
		.lpszMenuName = nullptr,
		.lpszClassName = _config.title.data(),
		.hIconSm = _config.iconSmile,
	};

	ATOM atom{ RegisterClassEx(&WNDCLASSEX_DESC) };
	assert(atom != 0 && "ウィンドウクラス登録に失敗");

	RECT windowRect{ 0, 0, _config.windowScreenSize.x, _config.windowScreenSize.y };
	BOOL succeed{ AdjustWindowRectEx(&windowRect, _config.clientStyle, _config.hasMenu, _config.clientStyleEx) };
	assert(succeed && "クライアント領域を考慮したウィンドウサイズ計算に失敗");

	Vec2Int windowSize
	{
		// 計算されたウィンドウのサイズ 横幅
		windowRect.right - windowRect.left,
		// 計算されたウィンドウのサイズ 高さ
		windowRect.bottom - windowRect.top,
	};

	HWND hWnd = CreateWindowEx(
		_config.clientStyleEx,
		_config.title.data(),
		_config.title.data(),
		_config.clientStyle,
		_config.initPosition.x,
		_config.initPosition.y,
		windowSize.x,
		windowSize.y,
		_config.hWndParent,
		nullptr,
		hInstance,
		nullptr);

	DWORD error;
	error = GetLastError();

	assert(hWnd != nullptr && "ウィンドウの作成に失敗");
	assert(IsWindow(hWnd) && "ウィンドウではないハンドルが作られている");

	ShowWindow(hWnd, SW_SHOWDEFAULT);  // ウィンドウを表示

	handle_ = hWnd;
	windowSize_ = windowSize;
	clientSize_ = _config.windowScreenSize;
}
