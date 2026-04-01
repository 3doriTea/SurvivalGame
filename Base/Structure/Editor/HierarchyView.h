#pragma once
#include "IEditorView.h"
#include <Component/GameObject.h>
#include <Component/Transform.h>
#include <Component/Transform2D.h>
#include "Common/ComponentSelector.h"

namespace GameBase::Editor
{
	class HierarchyView : public IEditorView
	{
		using ViewGameObjectTransform = View<Component::GameObject, Component::Transform>;
		using ViewGameObjectTransform2D = View<Component::GameObject, Component::Transform2D>;

		struct Object
		{
			Object() :
				Object{ INVALID_ENTITY }
			{}

			Object(const Entity _entity) :
				entity{ _entity },
				parent{},
				childs{}
			{}


			Entity entity;
			Entity parent;
			std::vector<Entity> childs;
		};

		

	public:
		HierarchyView();
		inline ~HierarchyView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		/// <param name=""></param>
		bool OnGUI(EntityRegistry&) override;

		/// <summary>
		/// 選択中のエンティティを取得
		/// </summary>
		/// <returns>選択中のエンティティ</returns>
		Entity GetSelectedEntity() const { return selectedEntity_; }

		void OnSelected(EntityRegistry& _registry, SelectedEvent& _event) override;

	private:
		void ShowNodeTree(ViewGameObjectTransform& _view, const Object& _obj);
		void ShowNodeTree(ViewGameObjectTransform2D& _view, const Object& _obj);

		/// <summary>
		/// オブジェクト作成のモーダルダイアログを表示
		/// </summary>
		void OpenModalCreateObject();

		/// <summary>
		/// 新規作成オブジェクト名が無効か
		/// </summary>
		/// <returns>無効 true / false</returns>
		bool IsInvalidCreateName();

		void CreateEntity(EntityRegistry& _registry);

	private:
		std::map<Entity, Object> objects_;
		Entity selectedEntity_;
		bool onSelectedEvent_;

		struct
		{
			std::array<char, Component::GAME_OBJECT_NAME_BUFFER_SIZE> name;
			ComponentSelector componentSelector;  // コンポーネント選択
		} createOptionsBuffer;
	};
}
