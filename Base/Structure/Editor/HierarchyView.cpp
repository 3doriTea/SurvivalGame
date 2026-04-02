#include "HierarchyView.h"
#include <System/TransformCalculator.h>
#include <System/Transform2DCalculator.h>
#include <System/SceneManager.h>
#include <System/SceneSaver.h>
#include "SelectedEvent.h"
#include <Component/Image.h>


GameBase::Editor::HierarchyView::HierarchyView() :
	selectedEntity_{ INVALID_ENTITY },
	onSelectedEvent_{},
	createOptionsBuffer{}
{}

bool GameBase::Editor::HierarchyView::OnGUI(EntityRegistry& _registry)
{
	onSelectedEvent_ = false;

	ImGui::Begin("シーン内オブジェクト親子関係");

	if (Get<System::SceneSaver>().HasUnsaved())
	{
		ImGui::Text("%s (未保存)", Get<System::SceneManager>().GetCurrentName().c_str());
	}
	else
	{
		ImGui::Text("%s", Get<System::SceneManager>().GetCurrentName().c_str());
	}

#pragma region オブジェクト作成

	if (ImGui::Button("オブジェクトを作成"))
	{
		OpenModalCreateObject();
	}

	ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("新規オブジェクト", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::IsWindowAppearing())
		{
			ImGui::SetKeyboardFocusHere();
		}

		ImGui::InputText("名前", createOptionsBuffer.name.data(), createOptionsBuffer.name.size());

		createOptionsBuffer.componentSelector.OnGUI();

		ImGui::Separator();

		ImGui::BeginDisabled(IsInvalidCreateName());
		if (ImGui::Button("作成 (Enter)", ImVec2{ 120, 0 }))
		{
			CreateEntity(_registry);

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("キャンセル (Esc)", ImVec2{ 120, 0 }))
		{
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Enter) && !IsInvalidCreateName())
		{
			CreateEntity(_registry);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

#pragma endregion

#pragma region ノードツリー 3D
	{
		objects_.clear();

		ViewGameObjectTransform view{ _registry.GetView<Component::GameObject, Component::Transform>() };
		for (const Entity entity : Get<System::TransformCalculator>().CalculationQueue())
		{
			auto [g, t]{view.At(entity) };

			objects_.emplace(entity, entity);

			if (t.parent != INVALID_ENTITY)
			{
				auto itr{ objects_.find(t.parent) };
				GB_ASSERT(itr != objects_.end()
					&& "順番が不正のため親子関係の登録に失敗");

				auto& [keyEntity, parentObj]{ *itr };
				parentObj.childs.push_back(entity);
				objects_[entity].parent = keyEntity;
			}
		}

		for (const auto& [entity, obj] : objects_)
		{
			if (obj.parent)
			{
				continue;  // 親がいるオブジェクトが現れたら以降は全部親あり
			}
			ShowNodeTree(view, obj);
		}
	}
#pragma endregion

#pragma region ノードツリー 2D
	{
		objects_.clear();

		ViewGameObjectTransform2D view{ _registry.GetView<Component::GameObject, Component::Transform2D>() };
		for (const Entity entity : Get<System::Transform2DCalculator>().CalculationQueue())
		{
			auto [g, t] {view.At(entity) };

			objects_.emplace(entity, entity);

			if (t.parent != INVALID_ENTITY)
			{
				auto itr{ objects_.find(t.parent) };
				GB_ASSERT(itr != objects_.end()
					&& "順番が不正のため親子関係の登録に失敗");

				auto& [keyEntity, parentObj] { *itr };
				parentObj.childs.push_back(entity);
				objects_[entity].parent = keyEntity;
			}
		}

		for (const auto& [entity, obj] : objects_)
		{
			if (obj.parent)
			{
				continue;  // 親がいるオブジェクトが現れたら以降は全部親あり
			}
			ShowNodeTree(view, obj);
		}
	}
#pragma endregion

	ImGui::End();

	return onSelectedEvent_;
}

void GameBase::Editor::HierarchyView::OnSelected(EntityRegistry& _registry, SelectedEvent& _event)
{
	if (_event.TryGetEvent<HierarchyView>())
	{
		return;  // 自分自身の選択イベントは無視
	}
	else
	{
		selectedEntity_ = INVALID_ENTITY;
	}
}

void GameBase::Editor::HierarchyView::ShowNodeTree(ViewGameObjectTransform& _view, const Object& _obj)
{
	ImGuiTreeNodeFlags flags
	{
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth
	};

	if (_obj.childs.empty())  // 子がないなら開けない
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	auto [g, t] { _view.At(_obj.entity) };

	bool isOpen
	{
		ImGui::TreeNodeEx(std::to_string(_obj.entity).c_str(), flags, "%s", g.name.data())
	};

	if (ImGui::IsItemClicked())
	{
		selectedEntity_ = _obj.entity;
		onSelectedEvent_ = true;
	}

	if (isOpen && !_obj.childs.empty())
	{
		for (Entity child : _obj.childs)
		{
			ShowNodeTree(_view, objects_[child]);
		}
		ImGui::TreePop();
	}
}

void GameBase::Editor::HierarchyView::ShowNodeTree(ViewGameObjectTransform2D& _view, const Object& _obj)
{
	ImGuiTreeNodeFlags flags
	{
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth
	};

	if (_obj.childs.empty())  // 子がないなら開けない
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	auto [g, t] { _view.At(_obj.entity) };

	bool isOpen
	{
		ImGui::TreeNodeEx(std::to_string(_obj.entity).c_str(), flags, "%s", g.name.data())
	};

	if (ImGui::IsItemClicked())
	{
		selectedEntity_ = _obj.entity;
		onSelectedEvent_ = true;
	}

	if (isOpen && !_obj.childs.empty())
	{
		for (Entity child : _obj.childs)
		{
			ShowNodeTree(_view, objects_[child]);
		}
		ImGui::TreePop();
	}
}

void GameBase::Editor::HierarchyView::OpenModalCreateObject()
{
	createOptionsBuffer.name.fill(0);
	createOptionsBuffer.componentSelector.ApplySetTemplateComponentFlags();
	ImGui::OpenPopup("新規オブジェクト");
}

bool GameBase::Editor::HierarchyView::IsInvalidCreateName()
{
	return createOptionsBuffer.name[0] == '\0';
}

void GameBase::Editor::HierarchyView::CreateEntity(EntityRegistry& _registry)
{
	createOptionsBuffer.componentSelector.ApplyAddRequiredComponentFlags();

	Entity entity{ _registry.CreateEntity() };

	_registry.AddComponents(entity, createOptionsBuffer.componentSelector.GetComponentFlags());

	auto& gameObject{ _registry.GetComponent<Component::GameObject>(entity) };
	gameObject.SetName(createOptionsBuffer.name.data());

	Get<System::SceneSaver>().SetDirty();
}
