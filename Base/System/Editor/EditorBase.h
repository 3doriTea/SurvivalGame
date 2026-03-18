#pragma once
#include "../SystemBase.h"
#include "../../Structure/Editor/IEditorView.h"
#include "../../Structure/Editor/SelectedEvent.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IEditorBase : public ISystemInterfaceBase
	{
	public:
		IEditorBase() = default;
		virtual ~IEditorBase() = default;

		virtual void RefSelectedEvent(const std::function<void(Editor::SelectedEvent&)>& _callback) = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class EditorBase : public SystemBase<EditorBase, IEditorBase>
	{
	public:
		EditorBase();
		~EditorBase();

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

		void RefSelectedEvent(const std::function<void(Editor::SelectedEvent&)>& _callback) override;

	private:
		Editor::SelectedEvent selectedEvent_;
		std::shared_ptr<void> onGUIEvent_;
		std::vector<std::shared_ptr<Editor::IEditorView>> pEditors_;

	};
}
