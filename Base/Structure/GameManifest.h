#pragma once

namespace GameBase
{
	struct GameManifest
	{
		std::string title;  // ゲームタイトル
		Vec3Int version;    // major, minor, patch バージョン情報
		std::string developer;  // 開発者名
		std::string copyright;  // 著作権情報
	};
}
