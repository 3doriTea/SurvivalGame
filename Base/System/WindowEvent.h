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

		/// <summary>
		/// ウィンドウ終了時イベント登録
		/// </summary>
		/// <param name="_callback">void(EventSubject<>&)></param>
		virtual void RefOnCloseEvent(
			const std::function<void(EventSubject<>&)>& _callback) = 0;

		/// <summary>
		/// ウィンドウ終了をキャンセルする
		/// </summary>
		virtual void CancelCloseWindow() = 0;
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
		void Update(EntityRegistry&) override;

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
		void RefOnCloseEvent(const std::function<void(EventSubject<>&)>& _callback) override;

		void CancelCloseWindow() override { plsCancelCloseWindow_ = true; }

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		bool plsCancelCloseWindow_;  // ウィンドウ終了キャンセル界隈
		ResponderEvent<HWND, UINT, WPARAM, LPARAM> event_;
		EventSubject<> onCloseEvent_;  // ウィンドウ終了時のイベント
		std::shared_ptr<void> defWindowProcEvent_;
		MSG peekedMessage_;

	};
}
