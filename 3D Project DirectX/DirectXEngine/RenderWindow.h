#pragma once
#include "DirectXHelp.h"
#include "Graphics.h"
class RenderWindow
{
public:
	bool Initialize(HINSTANCE hInstance, int width, int height, int nCmdShow);
	bool ProcessMessages();//render all messages, if window is closed it will return false
	//RenderWindow();
	HWND GetHWND()const;
	const XMFLOAT2 GetWindowHeightAndWidth();

	~RenderWindow();

private:
	void RegisterWindowClass();
	HWND handle = NULL;//handle to the window
	HINSTANCE hInstance = NULL; //Handle to application instance
	int width = 0;
	int height = 0;

};
