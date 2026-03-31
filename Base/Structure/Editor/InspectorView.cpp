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
			OpenModalEditComponent();
		}

		ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("新規オブジェクト", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::IsWindowAppearing())
			{
				ImGui::SetKeyboardFocusHere();
			}

			const char* types[]{ "空っぽ", "3Dオブジェクト", "UIオブジェクト" };
			if (ImGui::Combo("タイプ", &createOptionsBuffer.selected, types, IM_ARRAYSIZE(types)))
			{
				ApplyRequiredCreateObjectComponentFlags();
			}

			ImGui::BeginChild("機能要素の選択", ImVec2{ 0, 150 }, true);
			{
				for (ComponentIndex i = 0; i < ComponentRegistry::IndexCounter(); i++)
				{
					const std::string_view typeName{ ComponentRegistry::ComponentTypeNames()[i] };

					uint32_t* pFlags{};
					uint64_t offset{ 0ULL };
					if (i >= 32U)
					{
						pFlags = &createOptionsBuffer.componentFlags.upper;
						offset = 32ULL;
					}
					else
					{
						pFlags = &createOptionsBuffer.componentFlags.lower;
					}

					if (ImGui::CheckboxFlags(typeName.data(), pFlags, 1U << (i - offset)))
					{
						ApplyRequiredCreateObjectComponentFlags();
					}
				}
			}
			ImGui::EndChild();

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
	}

	#if 0
	Get<System::EditorBase>().RefSelectedEvent([&_registry](SelectedEvent& _event)
		{
			if (_event.TryGetEvent<HierarchyView>([&_registry](HierarchyView& _view)
			{
				Entity selectedEntity{ _view.GetSelectedEntity() };

				std::vector<ComponentIndex> componentIndices
				{
					_registry.GetComponentIndices(selectedEntity)
				};

				SchemaLoadBundle loadBundle{};
				for (ComponentIndex index : componentIndices)
				{
					YAML::Emitter emitter{};
					emitter << YAML::BeginMap;
					_registry.GetComponent(selectedEntity, index).OnSave(emitter, loadBundle);
					emitter << YAML::EndMap;

					Schema::GameComponent schema{};

					std::string yamlStr{ emitter.c_str() };
					schema.node = YAML::Load(yamlStr);
					static const std::string_view HEAD{ "struct " };
					schema.tag = ComponentRegistry::ComponentTypeNames().at(index).substr(HEAD.size());

					std::string build{ YamlBuilder{ loadBundle }.Write(schema).Build() };

					//schema.node
					//ImGui::Selectable(build.c_str());
					static char text[4096]{};
					memset(text, 0, sizeof(text));
					memcpy(text, build.c_str(), build.size());

					ImGui::PushStyleColor(ImGuiCol_FrameBg, {});
					ImGui::PushStyleColor(ImGuiCol_Border, {});
					ImGui::InputTextMultiline(std::format("##{}", schema.tag).c_str(), text, sizeof(text),
						ImVec2{ -FLT_MIN, ImGui::GetTextLineHeight() * 10 }, ImGuiInputTextFlags_ReadOnly);
					ImGui::PopStyleColor(2);
				}

			})) return;
		});
	#endif

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

void GameBase::Editor::InspectorView::OpenModalEditComponent()
{
	ImGui::OpenPopup("コンポーネントの編集");
}
