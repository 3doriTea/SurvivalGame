#pragma once

namespace GameBase
{
	/// <summary>
	/// UIの頂点
	/// </summary>
	struct alignas(16) UIVertex
	{
		inline UIVertex() :
			UIVertex{ Vec2::Zero(), Vec2::Zero() }
		{}

		inline UIVertex(
			const Vec2 _position,
			const Vec2 _uv) :
			position{ _position },
			uv{ _uv }	
		{}

		Vec2 position; uint32_t padding0{}, padding1{};
		Vec2 uv;       uint32_t padding2{}, padding3{};
	};
}
