#include "ComponentSelector.h"
#include <ComponentRegistry.h>

using namespace GameBase;

ComponentSelector::ComponentSelector() :
	selected_{},
	componentFlags_{}
{}

void ComponentSelector::OnGUI()
{
	
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
		createOptionsBuffer.componentFlags.full |=
			1ULL << ComponentRegistry::GetComponentIndex<Component::Transform>();
		break;
	case CreateObjectType::UI:
		createOptionsBuffer.componentFlags.full |=
			1ULL << ComponentRegistry::GetComponentIndex<Component::Transform>() |
			1ULL << ComponentRegistry::GetComponentIndex<Component::Image>();
		break;
	default:
		GB_ASSERT(false && "未実装の新規オブジェクトタイプ");
		break;
	}
}
