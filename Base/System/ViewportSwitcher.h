#pragma once
#include "../SystemBase.h"
#include "../Structure/RenderSurface.h"

namespace GameBase
{
	enum struct ViewportMode
	{
		Gamer,
		Editor,
	};
}

namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: ビューポートスイッチャー
	/// </summary>
	class IViewportSwitcher : public ISystemInterfaceBase
	{
	public:
		IViewportSwitcher() = default;
		virtual ~IViewportSwitcher() = default;

		/// <summary>
		/// ビューポートモードを変更する
		/// </summary>
		/// <param name="_mode">変更先のモード</param>
		virtual void Switch(const ViewportMode _mode) = 0;

		/// <summary>
		/// 描画対象の情報を参照する
		/// </summary>
		/// <param name="_mode">描画先</param>
		/// <param name="_callback">コールバック関数</param>
		virtual void RefAt(const ViewportMode _mode, const std::function<void(const RenderSurface&)>& _callback) = 0;
	};

	/// <summary>
	/// システム: ビューポートスイッチャー
	/// </summary>
	class ViewportSwitcher : public SystemBase<ViewportSwitcher, IViewportSwitcher>
	{
	public:
		ViewportSwitcher();
		~ViewportSwitcher();

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

		void Switch(const ViewportMode _mode) override;
		void RefAt(const ViewportMode _mode, const std::function<void(const RenderSurface&)>& _callback) override;

	private:
		ViewportMode currentMode_;  // 現在のモード

		struct
		{
			RenderSurface gamer;   // ゲーム用
			RenderSurface editor;  // エディタ用
		} renderSurface_;  // 描画先
	};
}
