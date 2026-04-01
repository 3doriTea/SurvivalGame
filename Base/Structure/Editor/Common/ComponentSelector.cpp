#include "ComponentSelector.h"
#include <ComponentRegistry.h>
#include <Component/GameObject.h>
#include <Component/Transform.h>
#include <Component/Transform2D.h>
#include <Component/Image.h>

using namespace GameBase;

ComponentSelector::ComponentSelector() :
	selected_{},
	componentFlags_{}
{}

void ComponentSelector::OnGUI()
{
	const char* types[]{ "空っぽ", "3Dオブジェクト" };
	if (ImGui::Combo("タイプ", &selected_, types, IM_ARRAYSIZE(types)))
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
				pFlags = &componentFlags_.upper;
				offset = 32ULL;
			}
			else
			{
				pFlags = &componentFlags_.lower;
			}

			if (ImGui::CheckboxFlags(typeName.data(), pFlags, 1U << (i - offset)))
			{
				ApplyRequiredCreateObjectComponentFlags();
			}
		}
	}
	ImGui::EndChild();
}

void ComponentSelector::ApplyRequiredCreateObjectComponentFlags()
{
	componentFlags_.full |=
		1ULL << ComponentRegistry::GetComponentIndex<Component::GameObject>();
	switch (selected_)
	{
	case CreateObjectType::Empty:
		break;
	case CreateObjectType::_3D:
		componentFlags_.full |=
			1ULL << ComponentRegistry::GetComponentIndex<Component::Transform>();
		break;
	case CreateObjectType::UI:
		componentFlags_.full |=
			1ULL << ComponentRegistry::GetComponentIndex<Component::Transform2D>() |
			1ULL << ComponentRegistry::GetComponentIndex<Component::Image>();
		break;
	default:
		GB_ASSERT(false && "未実装の新規オブジェクトタイプ");
		break;
	}
}
