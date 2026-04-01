#pragma once
#include "IEditorView.h"
#include "Inspector/Component.h"
#include "Common/ComponentSelector.h"

namespace GameBase::Editor
{
	class InspectorView : public IEditorView
	{
		enum struct Mode
		{
			None,            // 何も選択されていない
			ShowComponents,  // コンポーネントを選択中
			AssetItem,       // アセットアイテムを選択中
		};

	public:
		InspectorView();
		inline ~InspectorView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		/// <param name=""></param>
		bool OnGUI(EntityRegistry&) override;

		/// <summary>
		/// 何かしらが選択された
		/// </summary>
		/// <param name="_event"></param>
		void OnSelected(EntityRegistry& _registry, SelectedEvent& _event) override;

		void EditComponent(EntityRegistry& _registry);

	private:
		/// <summary>
		/// コンポーネントを編集するモーダルダイアログを表示
		/// </summary>
		void OpenModalEditComponent();

	private:
		Entity selectedEntity_;  // 選択されたエンティティ
		std::vector<Inspector::Component> components_;  // 選択されたエンティティのコンポーネント

		ComponentSelector componentSelector_;  // コンポーネント選択する

		Mode mode_;  // 選択中のモード
	};
}
