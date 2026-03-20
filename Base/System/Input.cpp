#include "pch.h"
#include "Input.h"
#include "GameTime.h"
#include "MainWindow.h"
#include <GameEvent/GameExit.h>


GameBase::System::Input::Input() :
	keyBoard_{},
	mouse_{},
	controller_{}
{
}

GameBase::System::Input::~Input()
{
}

void GameBase::System::Input::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		->Add(SystemRegistry::GetSystemIndex<MainWindow>())
		;
}

void GameBase::System::Input::Initialize()
{
	HRESULT hResult{};
	HWND hWnd{ Get<MainWindow>().GetHandle() };

#pragma region DirectInput8デバイス作成
	hResult = DirectInput8Create(
		GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<void**>(pDirectInput_.GetAddressOf()),
		nullptr);
	GB_ASSERT(SUCCEEDED(hResult) && "DirectInput8の作成に失敗");
	if (FAILED(hResult))
	{
		return;
	}
#pragma endregion

#pragma region キーボードデバイス準備
	hResult = pDirectInput_.Get()->CreateDevice(
		GUID_SysKeyboard,
		keyBoard_.pDevice.GetAddressOf(),
		nullptr);
	GB_ASSERT(SUCCEEDED(hResult) && "KeyBoardDeviceの作成に失敗");
	if (FAILED(hResult))
	{
		return;
	}

	hResult = keyBoard_.pDevice.Get()->SetDataFormat(&c_dfDIKeyboard);
	GB_ASSERT(SUCCEEDED(hResult) && "KeyBoardDeviceのフォーマット設定に失敗");
	if (FAILED(hResult))
	{
		return;
	}

	hResult = keyBoard_.pDevice.Get()->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	GB_ASSERT(SUCCEEDED(hResult) && "KeyBoardDeviceの干渉レベル設定に失敗");
	if (FAILED(hResult))
	{
		return;
	}
#pragma endregion

#pragma region マウスデバイス準備
	hResult = pDirectInput_.Get()->CreateDevice(
		GUID_SysMouse,
		mouse_.pDevice.GetAddressOf(),
		nullptr);
	GB_ASSERT(SUCCEEDED(hResult) && "MouseDeviceの作成に失敗");
	if (FAILED(hResult))
	{
		return;
	}

	hResult = mouse_.pDevice.Get()->SetDataFormat(&c_dfDIMouse);
	GB_ASSERT(SUCCEEDED(hResult) && "MouseDeviceのフォーマット設定に失敗");
	if (FAILED(hResult))
	{
		return;
	}

	hResult = mouse_.pDevice.Get()->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	GB_ASSERT(SUCCEEDED(hResult) && "MouseDeviceの干渉レベル設定に失敗");
	if (FAILED(hResult))
	{
		return;
	}
#pragma endregion
}

void GameBase::System::Input::Update(EntityRegistry&)
{
	if (Get<GameTime>().IsFrameDue() == false)
	{
		return;
	}

	// キーボード更新
	if (HRESULT hResult{ keyBoard_.pDevice.Get()->Acquire() };
		SUCCEEDED(hResult))
	{
		// 前フレームにコピー
		keyBoard_.statePrev = keyBoard_.stateCurr;

		keyBoard_.pDevice.Get()->GetDeviceState(
			static_cast<DWORD>(keyBoard_.stateCurr.size()),
			keyBoard_.stateCurr.data());
	}

	// マウス更新
	if (HRESULT hResult{ mouse_.pDevice.Get()->Acquire() };
		SUCCEEDED(hResult))
	{
		// 前フレームにコピー
		mouse_.statePrev = mouse_.stateCurr;

		mouse_.pDevice.Get()->GetDeviceState(
			sizeof(mouse_.stateCurr),
			&mouse_.statePrev);
	}
}

void GameBase::System::Input::Release()
{
}

bool GameBase::System::Input::IsPress(const KeyCode _keyCode)
{
	return keyBoard_.stateCurr[static_cast<uint32_t>(_keyCode)];
}

bool GameBase::System::Input::IsPressDown(const KeyCode _keyCode)
{
	return keyBoard_.stateCurr[static_cast<uint32_t>(_keyCode)]
		&& !keyBoard_.statePrev[static_cast<uint32_t>(_keyCode)];
}

bool GameBase::System::Input::IsPressUp(const KeyCode _keyCode)
{
	return !keyBoard_.stateCurr[static_cast<uint32_t>(_keyCode)]
		&& keyBoard_.statePrev[static_cast<uint32_t>(_keyCode)];
}
