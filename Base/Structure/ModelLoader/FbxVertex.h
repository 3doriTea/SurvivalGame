#pragma once

namespace GameBase
{
	/// <summary>
	/// FBXの頂点
	/// </summary>
	struct FbxVertex
	{
		Vec3 position; uint32_t padding0;
		Vec3 normal;   uint32_t padding1;
		Vec2 uv;       uint32_t padding2, padding3;
	};
}
