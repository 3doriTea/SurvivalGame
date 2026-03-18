#include "HierarchyView.h"
#include <System/TransformCalculator.h>


void GameBase::Editor::HierarchyView::OnGUI(EntityRegistry& _registry)
{


	ImGui::Begin("Hierarchy");

	ImGui::Text("Object List...");

	objects_.clear();

	ViewGameObjectTransform view{ _registry.GetView<Component::GameObject, Component::Transform>() };
	for (const Entity entity : Get<System::TransformCalculator>().CalculationQueue())
	{
		auto [g, t]{view.At(entity) };

		objects_.push_back(entity);

		if (t.parent != INVALID_ENTITY)
		{
			auto [parentG, parentT]{ view.At(t.parent) };
			auto itr
			{
				std::find_if(
					objects_.begin(),
					objects_.end(),
					[entity](const Object& _pObj){ return _pObj.entity == entity; })
			};

			GB_ASSERT(itr != objects_.end()
				&& "順番が不正のため親子関係の登録に失敗");

			itr->childs.push_back(&objects_.back());
			objects_.back().pParent = &(*itr);
		}
	}

	for (const Object& obj : objects_)
	{
		if (obj.pParent)
		{
			break;  // 親がいるオブジェクトが現れたら以降は全部親あり
		}
		ShowObjectTree(view, obj);
	}

	/*view.ForEach([](const Entity _entity, Component::GameObject& _gameObject, Component::Transform& _transform)
	{
		ImGui::Text("%s", _gameObject.name);
	});*/

	ImGui::End();
}

void GameBase::Editor::HierarchyView::ShowObjectTree(ViewGameObjectTransform& _view, const Object& obj)
{
	ImGuiTreeNodeFlags flags
	{
		ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth
	};

	if (obj.childs.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	auto [g, t]{ _view.At(obj.entity) };

	bool isOpen
	{
		ImGui::TreeNodeEx(
			std::format("{}", obj.entity).c_str(),
			flags,
			reinterpret_cast<const char*>(g.name.data()))
	};

	if (ImGui::IsItemClicked())
	{
		//selectedIndex_ = 
	}

	if (isOpen && !obj.childs.empty())
	{
		for (auto& child : obj.childs)
		{
			ShowObjectTree(_view, (*child));
		}
		ImGui::TreePop();  // 階層を一段戻る
	}
}
