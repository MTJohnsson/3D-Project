#pragma once
#include "Renderwindow.h"
#include "Graphics.h"
#include "Lib/Clock.h"
#include <Mouse.h>
#include <vector>

//#include <SimpleMath.h>
//using namespace DirectX::SimpleMath;

class Engine 
{
public:
	bool Initialize(HINSTANCE hInstance,int width, int height, int nCmdShow);
	bool ProcessMessage();//render all messages, if window is closed it will return false
	void Update();
	void RenderFrame();
	~Engine();

	std::unique_ptr<Mouse> mouse;
	XMFLOAT2 Lastposition;
private:
	RenderWindow render_window;
	Graphics gfx;
	Clock clock;

	std::vector<XMFLOAT4> mousePickInfo;	// rayOrigin, rayDir
};
