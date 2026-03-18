#pragma once
#include "IEditorView.h"
#include <Component/GameObject.h>
#include <Component/Transform.h>

namespace GameBase::Editor
{
	class HierarchyView : public IEditorView
	{
		using ViewGameObjectTransform = View<Component::GameObject, Component::Transform>;

		struct Object
		{
			Object(const Entity _entity) :
				entity{ _entity },
				pParent{},
				childs{}
			{}


			Entity entity;
			Object* pParent;
			std::vector<Object*> childs;
		};
	public:
		HierarchyView() {}
		inline ~HierarchyView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		/// <param name=""></param>
		void OnGUI(EntityRegistry&) override;

	private:
		/// <summary>
		/// ImGUIでオブジェクトツリーを表示する
		/// </summary>
		/// <param name="obj"></param>
		void ShowObjectTree(ViewGameObjectTransform& _view, const Object& obj);

	private:
		std::vector<Object> objects_;
		int selectedIndex_;

	};
}
