#include "EditorBase.h"
#include "../EditorGui.h"
#include "../TextureRegistry.h"
#include "../TransformCalculator.h"

#include "../../Structure/Editor/AssetsView.h"
#include "../../Structure/Editor/HierarchyView.h"
#include "../../Structure/Editor/InspectorView.h"


GameBase::System::EditorBase::EditorBase()
{}

GameBase::System::EditorBase::~EditorBase()
{
}

void GameBase::System::EditorBase::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<EditorGui>())
		->Add(SystemRegistry::GetSystemIndex<TextureRegistry>())
		->Add(SystemRegistry::GetSystemIndex<TransformCalculator>())
	;
}

void GameBase::System::EditorBase::Initialize()
{
	pEditors_.emplace_back(std::make_shared<Editor::AssetsView>());
	pEditors_.emplace_back(std::make_shared<Editor::HierarchyView>());
	pEditors_.emplace_back(std::make_shared<Editor::InspectorView>());

	Get<EditorGui>().OnGUI([this](EventSubject<EntityRegistry&>& _event)
	{
		onGUIEvent_ = _event.get()->Connect([this](EntityRegistry& _registry)
		{
				static bool opt_fullscreen = true;
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
				if (opt_fullscreen)
				{
					const ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::SetNextWindowPos(viewport->WorkPos);
					ImGui::SetNextWindowSize(viewport->WorkSize);
					ImGui::SetNextWindowViewport(viewport->ID);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
					window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
					window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				}

				ImGui::Begin("MyDockSpace", nullptr, window_flags);
				if (opt_fullscreen) ImGui::PopStyleVar(2);

				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				bool selectedOnce{ false };
				for (const std::shared_ptr<Editor::IEditorView>& p : pEditors_)
				{
					bool selected{ p.get()->OnGUI(_registry) };

					if (selected && !selectedOnce)
					{

						selectedEvent_.Update(p, std::type_index{ typeid(*p) });
						selectedOnce = true;
					}
				}

				// なにかしらが選択された瞬間
				if (selectedOnce)
				{
					for (const std::shared_ptr<Editor::IEditorView>& p : pEditors_)
					{
						p.get()->OnSelected(_registry, selectedEvent_);
					}
				}

				ImGui::Begin("Game View");
				ImGui::End();

				ImGui::Begin("Scene View");
				ImGui::End();

				

				ImGui::End();
		});
	});
}

void GameBase::System::EditorBase::Update(EntityRegistry&)
{
}

void GameBase::System::EditorBase::Release()
{}

void GameBase::System::EditorBase::RefSelectedEvent(const std::function<void(Editor::SelectedEvent&)>& _callback)
{
	_callback(selectedEvent_);
}
