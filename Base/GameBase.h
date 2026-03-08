#pragma once

extern void Main();

namespace GameBase
{
	class Game final
	{
	public:
		Game();
		~Game() = default;

		bool Start();
		bool Update();
		bool End();

	private:
		std::vector<int> systemOrderIndices_;  // システムの順番インデックス
		std::vector<std::type_info> systemsTypeinfo_;  // システムの型情報
	};
}

using namespace GameBase;
