#include <GameBase.h>
#include <Debugger.h>
#include <System/Input.h>

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
	while (game.Update())
	{
		if (Get<System::Input>().IsPressDown(KeyCode::Escape)
			&& Get<System::Input>().IsPress(KeyCode::LeftShift))
		{
			break;
		}
	}
	LogEnd();

	if (!game.End())
	{
		Log("終了失敗");
	}

	LogEnd();
	Debugger::LogWriteOutFile("./", "");
}

