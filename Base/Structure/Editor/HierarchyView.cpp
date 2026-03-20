#include "HierarchyView.h"
#include <System/TransformCalculator.h>
#include "SelectedEvent.h"


GameBase::Editor::HierarchyView::HierarchyView() :
	selectedEntity_{ INVALID_ENTITY },
	onSelectedEvent_{},
	createOptionsBuffer{}
{}

bool GameBase::Editor::HierarchyView::OnGUI(EntityRegistry& _registry)
{
	onSelectedEvent_ = false;

	ImGui::Begin("シーン内オブジェクト親子関係");

#pragma region オブジェクト作成

	if (ImGui::Button("オブジェクト作成"))
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

		const char* types[]{ "空っぽ", "3Dオブジェクト" };
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
		selectedEntity_ = obj.entity;
		onSelectedEvent_ = true;
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

void GameBase::Editor::HierarchyView::OpenModalCreateObject()
{
	createOptionsBuffer.name.fill(0);
	createOptionsBuffer.componentFlags.full = 0;
	ApplyRequiredCreateObjectComponentFlags();
	ImGui::OpenPopup("新規オブジェクト");
}

bool GameBase::Editor::HierarchyView::IsInvalidCreateName()
{
	return createOptionsBuffer.name[0] == '\0';
}

void GameBase::Editor::HierarchyView::ApplyRequiredCreateObjectComponentFlags()
{
	createOptionsBuffer.componentFlags.full |=
		1ULL << ComponentRegistry::GetComponentIndex<Component::GameObject>();
	switch (createOptionsBuffer.selected)
	{
	case CreateObjectType::Empty:
		break;
	case CreateObjectType::_3D:
		createOptionsBuffer.componentFlags.full |=
			1ULL << ComponentRegistry::GetComponentIndex<Component::Transform>();
		break;
	default:
		GB_ASSERT(false && "未実装の新規オブジェクトタイプ");
		break;
	}
}

void GameBase::Editor::HierarchyView::CreateEntity(EntityRegistry& _registry)
{
	ApplyRequiredCreateObjectComponentFlags();

	Entity entity{ _registry.CreateEntity() };

	_registry.AddComponents(entity, createOptionsBuffer.componentFlags.full);

	auto& gameObject{ _registry.GetComponent<Component::GameObject>(entity) };
	gameObject.SetName(createOptionsBuffer.name.data());
}
