#include "EditorGui.h"
#include "WindowEvent.h"
#include "Direct3D.h"
#include "MainWindow.h"
#include "GameTime.h"
#include "Renderer.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

GameBase::System::EditorGui::EditorGui()
{
}

GameBase::System::EditorGui::~EditorGui()
{
}

void GameBase::System::EditorGui::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
	->Add(SystemRegistry::GetSystemIndex<WindowEvent>())
	->Add(SystemRegistry::GetSystemIndex<Direct3D>())
	->Add(SystemRegistry::GetSystemIndex<MainWindow>())
	->Add(SystemRegistry::GetSystemIndex<GameTime>())
	->Add(SystemRegistry::GetSystemIndex<Renderer>())
	;
}

void GameBase::System::EditorGui::Initialize()
{
	Debugger::Log("こんにちは！初期化したよ！");

	Get<WindowEvent>().WndProcEvent(
		[this](ResponderEvent<HWND, UINT, WPARAM, LPARAM>& _event) -> WndProcEventResult
		{
			wndProcEvent_ = _event.Connect([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
				{
					return static_cast<WndProcEventResult>(ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam));
				});

			return WndProcEventResult::Pass;
		});

	ImGui_ImplWin32_EnableDpiAwareness();
	float main_scale
	{
		ImGui_ImplWin32_GetDpiScaleForMonitor(MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY))
	};

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() };
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
	io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
	io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(Get<MainWindow>().GetHandle());
	Get<Direct3D>().Ref([](const ComPtr<ID3D11Device>& _pDevice, const ComPtr<ID3D11DeviceContext>& _pContext)
		{
			ImGui_ImplDX11_Init(_pDevice.Get(), _pContext.Get());
		});

	Get<Renderer>().OnBegin([this](Event<>& _event)
		{
			renderBeginEvent_ = _event.Connect([]()
				{
					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();
				});
		});
	Get<Renderer>().OnEnd([this](Event<>& _event)
		{
			renderEndEvent_ = _event.Connect([]()
				{
					ImGui::Render();
					//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
					//g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
					//g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

					ImGuiIO& io{ ImGui::GetIO() };
					if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
					{
						ImGui::UpdatePlatformWindows();
						ImGui::RenderPlatformWindowsDefault();
					}
				});
		});

	Get<Renderer>().OnRender([this](Event<>& _event)
		{
			renderEvent_ = _event.Connect([]()
				{
					ImGui::Begin("Testing");

					ImGui::Text("format string!!");

					bool demo{ true };
					ImGui::ShowDemoWindow(&demo);

					ImGui::End();
				});
		});
}

void GameBase::System::EditorGui::Update()
{
	if (!Get<GameTime>().IsFrameDue())
	{
		return;
	}

	Debugger::Log("こんにちは！更新中だよ！");
}

void GameBase::System::EditorGui::Release()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
