#include <GameBase.h>
#include <Debugger.h>

void Main()
{
	using Debugger::Log;
	using Debugger::LogBegin;
	using Debugger::LogEnd;

	LogBegin("Main");

	Game game{};
	if (!game.Start())
	{
		Log("スタートに失敗");
		Debugger::LogWriteOutFile("./", "");
		return;
	}

	Log("こんにちは");

	LogBegin("Game");
	while (game.Update()) {}
	LogEnd();

	if (!game.End())
	{
		Log("終了失敗");
	}

	LogEnd();
	Debugger::LogWriteOutFile("./", "");
}

