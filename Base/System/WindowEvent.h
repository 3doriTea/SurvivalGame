#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	using WndProcEventResult = ResponderEvent<HWND, UINT, WPARAM, LPARAM>::Result;

	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	struct IWindowEvent : ISystemInterfaceBase
	{
		IWindowEvent() = default;
		virtual ~IWindowEvent() = default;

		/// <summary>
		/// WinProcイベントを参照する
		/// </summary>
		/// <param name="_callback">void(Event<HWND,UINT,WPARAM, LPARAM>&) コールバック</param>
		virtual WndProcEventResult WndProcEvent(
			const std::function<WndProcEventResult(
				ResponderEvent<HWND, UINT, WPARAM, LPARAM>&)>& _callback) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class WindowEvent : public SystemBase<WindowEvent, IWindowEvent>
	{
	public:
		WindowEvent();
		~WindowEvent();

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		/// <summary>
		/// ウィンドウプロシージャの関数ポインタを取得する
		/// </summary>
		/// <returns>関数ポインタ</returns>
		WNDPROC GetWndProc() const { return WndProc; }

		WndProcEventResult WndProcEvent(
			const std::function<WndProcEventResult(
				ResponderEvent<HWND, UINT, WPARAM, LPARAM>&)>& _callback) override;

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		ResponderEvent<HWND, UINT, WPARAM, LPARAM> event_;
		std::shared_ptr<void> defWindowProcEvent_;
		MSG peekedMessage_;
	};
}
