#include "InspectorView.h"
#include <System/Editor/EditorBase.h>
#include "HierarchyView.h"


GameBase::Editor::InspectorView::InspectorView()
{}

bool GameBase::Editor::InspectorView::OnGUI(EntityRegistry& _registry)
{
	ImGui::Begin("Inspector");

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
					_registry.GetComponent(selectedEntity, index).OnSave(emitter, loadBundle);
					ImGui::Text(emitter.c_str());
				}

			})) return;
		});

	ImGui::Text("Components...");
	ImGui::End();

	return false;
}
