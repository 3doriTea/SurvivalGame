#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: ImGuiを使ったエディタ
	/// </summary>
	struct IEditorGui : ISystemInterfaceBase
	{
		IEditorGui() = default;
		virtual ~IEditorGui() = default;
	};

	/// <summary>
	/// システム: ImGuiを使ったエディタ
	/// </summary>
	class EditorGui : public SystemBase<EditorGui, IEditorGui>
	{
	public:
		EditorGui();
		~EditorGui();

		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

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

	private:
		std::shared_ptr<void> wndProcEvent_;
		std::shared_ptr<void> renderBeginEvent_;
		std::shared_ptr<void> renderEvent_;
		std::shared_ptr<void> renderEndEvent_;

	};
}
