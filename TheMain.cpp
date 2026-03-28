#include <GameBase.h>
#include <Debugger.h>
#include <System/Input.h>

//#include <System/Editor/EditorBase.h>
//template class GameBase::SystemBase<GameBase::System::EditorBase, GameBase::System::IEditorBase>;

void Main()
{
	using Debugger::Log;
	using Debugger::LogBegin;
	using Debugger::LogEnd;

	LogBegin("Main");

	Game game{ RunMode::Editor };
	if (!game.Start("./Assets/Scenes/TestScene.scene.yaml"))
	{
		Log("スタートに失敗");
		Debugger::LogWriteOutFile("./Logs/", "");
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
	Debugger::LogWriteOutFile("./Logs/", "");
}

