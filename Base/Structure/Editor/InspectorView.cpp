#include "InspectorView.h"
#include <System/Editor/EditorBase.h>
#include "HierarchyView.h"
#include "../AssetLoader/YamlBuilder.h"
#include <System/SchemaLinker.h>


GameBase::Editor::InspectorView::InspectorView() :
	mode_{ Mode::None },
	selectedEntity_{ INVALID_ENTITY }
{}

bool GameBase::Editor::InspectorView::OnGUI(EntityRegistry& _registry)
{
	ImGui::Begin("Inspector");

	if (mode_ == Mode::ShowComponents)
	{
		const auto& loadBundle{ Get<System::SchemaLinker>().GetSchemaLoadBundle() };

		

		for (auto& component : components_)
		{
			YAML::Emitter emitter{};
			emitter << YAML::BeginMap;
			_registry.GetComponent(component.ENTITY, component.COMPONENT_INDEX).OnSave(
				emitter,
				loadBundle);
			emitter << YAML::EndMap;

			Schema::GameComponent schema{};

			std::string yamlStr{ emitter.c_str() };
			schema.node = YAML::Load(yamlStr);
			static const std::string_view HEAD{ "struct " };
			schema.tag = ComponentRegistry::ComponentTypeNames()
				.at(component.COMPONENT_INDEX)
				.substr(HEAD.size());

			std::string build{ YamlBuilder{ loadBundle }.Write(schema).Build() };

			if (component.IsUpdatedShow(_registry, build, loadBundle))
			{  // 値が変更された
				
			}
		}

#pragma region コンポーネントの編集
		if (ImGui::Button("コンポーネントの編集"))
		{
			OpenModalEditComponent(_registry);
		}

		ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("コンポーネント編集", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::IsWindowAppearing())
			{
				ImGui::SetKeyboardFocusHere();
			}

			componentSelector_.OnGUI();

			ImGui::Separator();

			ImGui::SetItemDefaultFocus();

			if (ImGui::Button("変更 (Enter)", ImVec2{ 120, 0 }))
			{
				EditComponent(_registry);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("キャンセル (Esc)", ImVec2{ 120, 0 }))
			{
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				EditComponent(_registry);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
#pragma endregion
	}

	ImGui::Text("Components...");

	ImGui::End();
	return false;
}

void GameBase::Editor::InspectorView::OnSelected(EntityRegistry& _registry, SelectedEvent& _event)
{
	if (_event.TryGetEvent<HierarchyView>([&_registry, this](HierarchyView& _view)
	{
		components_.clear();

		selectedEntity_ = _view.GetSelectedEntity();
		std::vector<ComponentIndex> componentIndices
		{
			_registry.GetComponentIndices(selectedEntity_)
		};

		for (ComponentIndex index : componentIndices)
		{
			components_.emplace_back(selectedEntity_, index);
		}
		mode_ = Mode::ShowComponents;
	})) return;

	mode_ = Mode::None;
}

void GameBase::Editor::InspectorView::EditComponent(EntityRegistry& _registry)
{
	Signature current{ _registry.GetComponentsMask(selectedEntity_) };
	Signature request{ componentSelector_.GetComponentFlags() };
	Signature toRemove{ current & ~request };
	Signature toAdd{ ~current & request };

	if ((toRemove | toAdd).any())
	{
		return;  // 変更無しのため何もしない
	}

	for (ComponentIndex index = 0; index < ComponentRegistry::IndexCounter(); index++)
	{
		if (toRemove.test(index))
		{
			_registry.RemoveComponent(selectedEntity_, index);
		}
		if (toAdd.test(index))
		{
			_registry.AddComponent(selectedEntity_, index);
		}
	}
}

void GameBase::Editor::InspectorView::OpenModalEditComponent(EntityRegistry& _registry)
{
	componentSelector_.SetComponentFlags(_registry.GetComponentsMask(selectedEntity_));

	ImGui::OpenPopup("コンポーネント編集");
}
