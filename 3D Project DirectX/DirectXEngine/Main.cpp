#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "Engine.h"
#include "DirectXHelp.h"

int WINAPI wWinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevUnstance, 
	LPWSTR liCmdLine,
	int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;
	if (engine.Initialize(hInstance, 1280, 1024, nCmdShow))
	{
		
		while (engine.ProcessMessage())
		{
			engine.Update();
			engine.RenderFrame();
		}
	}
	return 0;
}