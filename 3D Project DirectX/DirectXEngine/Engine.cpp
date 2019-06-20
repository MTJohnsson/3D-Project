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
	
	if (state.rightButton)
	{
 		mouse->SetMode(Mouse::MODE_RELATIVE);
		gfx.camera->AdjustRotation((float)state.y * 0.05f * dt,
			(float)state.x * 0.05f * dt, 0);
	}
	// Mouse picking
	else if (state.leftButton)
	{
		// VIEWPORT SPACE, aka SCREEN SPACE
		float mouseX = state.x;
		float mouseY = state.y;

		const XMMATRIX& proj = gfx.camera->GetProjectionMatrix();
		Vector4 row0(proj.r[0]);
		Vector4 row1(proj.r[1]);


		// normalized devise space
		mouseX = (((2.0f * mouseX) / render_window.GetWindowHeightAndWidth().x) - 1) / row0.x;
		mouseY = -(((2.0f * mouseY) / render_window.GetWindowHeightAndWidth().y) - 1) / row1.y;
	

		// NDC, PROJECTION SPACE
		Vector4 projCoord(mouseX, mouseY, 0.f, 1.f);

		Vector3 rayOrigin(0.f, 0.f, 0.f);			// q in slide 16
		Vector3 rayDir(mouseX, mouseY, 1.f);	// u in slide 16 (direction)

		// VIEW SPACE
		XMMATRIX invertedProjection = XMMatrixInverse(NULL, gfx.camera->GetProjectionMatrix());
		Vector4 viewCoord = XMVector4Transform(projCoord, invertedProjection);
		
		// WORLD SPACE
		XMMATRIX invertedView = XMMatrixInverse(NULL, gfx.camera->GetViewMatrix()); // V^-1

		Vector4 rayOriginW = XMVector4Transform(Vector4(rayOrigin.x, rayOrigin.y, rayOrigin.z, 1.f), invertedView); 
		Vector4 rayDirW    = XMVector4Transform(Vector4(rayDir.x, rayDir.y, rayDir.z, 0.f), invertedView); 

		mousePickInfo.emplace_back(XMFLOAT4(rayOriginW.x, rayOriginW.y, rayOriginW.z, rayOriginW.w));
		mousePickInfo.emplace_back(XMFLOAT4(rayDirW.x, rayDirW.y, rayDirW.z, rayDirW.w));

		//Vector3 campos = gfx.camera->GetPositionFloat3();

	}
	else
	{
		mousePickInfo.clear();
		mouse->SetMode(Mouse::MODE_ABSOLUTE);
		mouse->SetVisible(true);
	}

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
	if (GetAsyncKeyState(VK_CONTROL)) { //backspace 
		gfx.camera->AdjustPosition(0.0f, -0.01f * dt, 0.0f);
	}

	gfx.RenderFrame(dt, mousePickInfo);
}

void Engine::RenderFrame()
{
}

Engine::~Engine()
{
	
}
