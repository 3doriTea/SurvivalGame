#pragma once
#include "../SystemBase.h"
#include "Structure/CreateWindowConfig.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: メインウィンドウ
	/// </summary>
	class IMainWindow : public ISystemInterfaceBase
	{
	public:
		IMainWindow() = default;
		virtual ~IMainWindow() = default;

		/// <summary>
		/// メインウィンドウのクライアントサイズを取得する
		/// </summary>
		/// <returns>クライアントサイズ</returns>
		virtual Vec2Int GetClientSize() const = 0;
		/// <summary>
		/// メインウィンドウのウィンドウサイズを取得する
		/// </summary>
		/// <returns>ウィンドウサイズ</returns>
		virtual Vec2Int GetWindowSize() const = 0;
		/// <summary>
		/// ウィンドウのハンドルを取得する
		/// </summary>
		/// <returns></returns>
		virtual HWND GetHandle() const = 0;
	};

	/// <summary>
	/// システム: メインウィンドウ
	/// </summary>
	class MainWindow : public SystemBase<MainWindow, IMainWindow>
	{
	public:
		MainWindow();
		~MainWindow();

		// 利用する参照があるときに使用
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

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
		/// メインウィンドウを作る
		/// </summary>
		/// <param name="_config">作るウィンドウの設定</param>
		void Create(const CreateWindowConfig& _config);

		HWND GetHandle() const override { return handle_; }
		Vec2Int GetClientSize() const override { return clientSize_; }
		Vec2Int GetWindowSize() const override { return windowSize_; }

	private:
		Vec2Int clientSize_;
		Vec2Int windowSize_;
		HWND handle_;
	};
}
