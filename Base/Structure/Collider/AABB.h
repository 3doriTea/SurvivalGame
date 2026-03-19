#pragma once

namespace GameBase
{
	/// <summary>
	/// 軸並行バウンディングボックス
	/// </summary>
	struct AABB
	{
		Vec3 min;  // 最小点
		Vec3 max;  // 最大点
	};
}
