#pragma once

namespace GameBase
{
	enum ComponentType : uint32_t
	{
		COMPONENT_TYPE_MAX = 64,
	};

	using Signature = std::bitset<COMPONENT_TYPE_MAX>;
}
