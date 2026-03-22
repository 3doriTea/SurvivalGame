#include "WindowEvent.h"
#include <GameEvent/GameExit.h>

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

void GameBase::System::WindowEvent::RefOnCloseEvent(const std::function<void(EventSubject<>&)>& _callback)
{
	_callback(onCloseEvent_);
}

GameBase::System::WindowEvent::WindowEvent() :
	defWindowProcEvent_{},
	peekedMessage_{},
	onCloseEvent_{ Event<>::Create() },
	plsCancelCloseWindow_{ false }
{
}

GameBase::System::WindowEvent::~WindowEvent()
{
}

void GameBase::System::WindowEvent::Initialize()
{
	defWindowProcEvent_ = event_.Connect([this](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CLOSE:
				onCloseEvent_.get()->Invoke();
				if (plsCancelCloseWindow_)
				{
					plsCancelCloseWindow_ = false;
					// ウィンドウ終了キャンセル
					return WndProcEventResult::Pass;
				}
				return static_cast<WndProcEventResult>(DefWindowProc(hWnd, msg, wParam, lParam));
			case WM_DESTROY:
				GameEvent::Emplace<GameEvent::GameExit>();
				return static_cast<WndProcEventResult>(DefWindowProc(hWnd, msg, wParam, lParam));
			default:
				return static_cast<WndProcEventResult>(DefWindowProc(hWnd, msg, wParam, lParam));
			}

			return WndProcEventResult::Pass;
		});
}

void GameBase::System::WindowEvent::Update(EntityRegistry&)
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
