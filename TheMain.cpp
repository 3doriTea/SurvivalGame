#include <GameBase.h>
//#include <Debugger.h>

void Main()
{
	Game game{};
	game.Start();

	//using Debugger::Log;
	//using Debugger::LogBegin;
	//using Debugger::LogEnd;

	//LogBegin("Main");

	//Log("こんにちは");

	//LogEnd();

	while (game.Update()) {}

	game.End();
}

