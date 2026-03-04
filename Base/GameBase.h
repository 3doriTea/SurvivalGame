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
	class Game final
	{
	public:
		Game() = default;
		~Game() = default;

		bool Start();
		bool Update();
		bool End();

	private:
	};
}

using namespace GameBase;
