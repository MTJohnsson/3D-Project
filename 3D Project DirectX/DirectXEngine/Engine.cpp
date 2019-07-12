#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, int width, int height, int nCmdShow)
{
	clock.Start();
	mouse = std::make_unique<Mouse>();

	if (!this->render_window.Initialize(hInstance,width, height, nCmdShow))
	{
		return false;
	}
	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
	{
		return false;
	}
	mouse->SetWindow(render_window.GetHWND());
	return true;

}

bool Engine::ProcessMessage()
{
	return this->render_window.ProcessMessages();	
}

void Engine::Update()
{
	float dt = clock.getMilisecondsElapsed();
	clock.Restart();

	auto state = mouse->GetState();
	bool mouseClicked = false;
	if (state.rightButton)
	{
		mouse->SetMode(Mouse::MODE_RELATIVE);
		gfx.camera->AdjustRotation((float)state.y * 0.005f * dt,
			(float)state.x * 0.005f * dt, 0);
	}
	// Mouse picking
	else if (state.leftButton)
	{
		mouseClicked = true;
		// VIEWPORT SPACE, aka SCREEN SPACE
		float mouseX = state.x;
		float mouseY = state.y;

		gfx.RenderFrame(dt, mouseX, mouseY);
	}
	else
	{
		mousePickInfo.clear();
		mouse->SetMode(Mouse::MODE_ABSOLUTE);
		mouse->SetVisible(true);
	}
	if (!mouseClicked)
		gfx.RenderFrame(dt, -1, -1);
	
	if (GetAsyncKeyState(0x41)) {//A
		XMFLOAT3 adjust(-0.01f * dt, 0.0f, 0.0f);
		gfx.camera->AdjustPosition(gfx.camera->GetLeftVector() * 0.006 * dt);
	}
	if (GetAsyncKeyState(0x44)) { //D
		XMFLOAT3 adjust(0.01f * dt, 0.0f, 0.0f);
		gfx.camera->AdjustPosition(gfx.camera->GetRightVector() * 0.006 * dt);
	}
	if (GetAsyncKeyState(0x57)) {//W
		XMFLOAT3 adjust(0.0f, 0.0f, 0.01f * dt);
		gfx.camera->AdjustPosition(gfx.camera->GetForwardVector()*0.006*dt);
	}
	if (GetAsyncKeyState(0x53)) { //S
		XMFLOAT3 adjust(0.00f, 0.0f, -0.01f * dt);
		gfx.camera->AdjustPosition(gfx.camera->GetBackwardVector() * 0.006 * dt);
	}
	if (GetAsyncKeyState(VK_SPACE)) { //backspace 
		gfx.camera->AdjustPosition(0.0f, 0.01f * dt, 0.0f);
	}
}

void Engine::RenderFrame()
{
}

Engine::~Engine()
{
	
}
