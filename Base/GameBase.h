#pragma once

extern void Main();

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Main();
}

namespace GameBase
{
	class Game
	{
	public:
		Game();
		~Game();

		bool Start();
		bool Update();

	private:
	};
}

using namespace GameBase;
