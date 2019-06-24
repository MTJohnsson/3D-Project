#include "Renderwindow.h"

#include "wrl/client.h"
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment (lib, "d3dcompiler.lib")
#include "Mouse.h"
bool RenderWindow::Initialize(HINSTANCE hInstance, int width, int height, int nCmdShow)
{
	
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->RegisterWindowClass();

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(this->handle, nCmdShow);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	// Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	while (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		this->handle, //Handle to window we are checking messages for
		0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
		0,    //Maximum Filter Msg Value
		PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
	}

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; //Message processing loop takes care of destroying this window
			//UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}

const XMFLOAT2 RenderWindow::GetWindowHeightAndWidth()
{
	return XMFLOAT2(width, height);
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		//	UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

//imgui
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// check if IMGUI can handle the message (when we click INSIDE ImGui
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_ACTIVATEAPP:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = "BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex)) {
		//
	}
	RECT rc;
	rc.left = 50;
	rc.top = 50;
	rc.right = rc.left + width;
	rc.bottom = rc.top + height;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
	this->handle = CreateWindow(
		"BTH_D3D_DEMO",
		"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		rc.left,
		rc.top,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		this->hInstance,
		nullptr);

	
	if (this->handle == NULL)
	{
		MessageBox(NULL, "Handle Failed.", 
			"handle Initialisation Error", MB_OK);
	}

}