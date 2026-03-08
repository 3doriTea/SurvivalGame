#include "WindowEvent.h"

LRESULT WINAPI GameBase::System::WindowEvent::WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	GameBase::System::WndProcEventResult result
	{
		GameBase::System::Get<GameBase::System::WindowEvent>().WndProcEvent([_hWnd, _msg, _wParam, _lParam](auto& _event)
		{
			return _event.Invoke(_hWnd, _msg, _wParam, _lParam);
		})
	};

	return static_cast<LRESULT>(result);
}

GameBase::System::WindowEvent::WindowEvent() :
	defWindowProcEvent_{},
	peekedMessage_{}
{
}

GameBase::System::WindowEvent::~WindowEvent()
{
}

void GameBase::System::WindowEvent::Initialize()
{
	defWindowProcEvent_ = event_.Connect([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_DESTROY:
				break;
			default:
				return static_cast<WndProcEventResult>(DefWindowProc(hWnd, msg, wParam, lParam));
			}

			return WndProcEventResult::Pass;
		});
}

void GameBase::System::WindowEvent::Update()
{
	if (PeekMessage(&peekedMessage_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&peekedMessage_);
		DispatchMessage(&peekedMessage_);
	}
}

void GameBase::System::WindowEvent::Release()
{
}

GameBase::System::WndProcEventResult GameBase::System::WindowEvent::WndProcEvent(
	const std::function<WndProcEventResult(
		ResponderEvent<HWND, UINT, WPARAM, LPARAM>&)>& _callback)
{
	return _callback(event_);
}
