#include "D3DApp.h"

namespace 
{
	//Create global pointer to a DXApp object
	//This will be used to forward messages from a global space to
	//our user define message procedure. This is necessary due to the fact that
	//we cant create a method defined as WNDPROC.
	D3DApp* g_pD3DApp = NULL;
}

D3DApp::D3DApp(HINSTANCE hInstance)
{
	m_hAppInstance = hInstance;
	m_hAppWindow = NULL;
	m_AppTitle = "Penguin";
	m_nClientWidth = 600;
	m_nClientHeight = 600;
	bFullScreen = false;
	m_dwWindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX; //Standard non-resizeable window
	g_pD3DApp = this; //Set our global pointer
}

int D3DApp::Run()
{
	MSG msg = { 0 };
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// here goes rendering and updating..
		else 
		{
			//update
			//render
		}
			
	}
	//Now when the application finally finishes, we need to return
	//the error code given from our application
	return static_cast<int>(msg.wParam); //The error code is stored in the wParam member of our msg struct
}

bool D3DApp::Init()
{
	if(!InitializeMainwindow())
		return false;
	else
		return true;
}

LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // custom message handler
{
	if(g_pD3DApp) // pass on to lpfnWndProc for processing
		g_pD3DApp->lpfnWndProc(hwnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT __stdcall D3DApp::lpfnWndProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
	}
	//Always return the default window procedure if we don't catch anything
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



bool D3DApp::InitializeMainwindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbClsExtra = 0; //no extra bytes
	wcex.cbWndExtra = 0; //no extra bytes
	wcex.cbSize = sizeof(WNDCLASSEX); //set size in bytes
	wcex.style = CS_HREDRAW | CS_VREDRAW; //Basically states that window should be redraw both HORIZ. and VERT.
	wcex.hInstance = m_hAppInstance; //Set handle to application instance;
	wcex.lpfnWndProc = (WNDPROC)WndProc; //Set message procedure to our globally defined one
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); //Set window icon (standard application icon)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //Set window arrow (standard windows arrow)
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Set clear background
	wcex.lpszClassName = "WIN32WINDOWCLASS"; //Name it w.e you like. 
	wcex.lpszMenuName = NULL; //We are not using a menu at this time.
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //Set small window icon (standard application icon)

	if(!RegisterClassEx(&wcex))
	{
		MessageBoxA(NULL, "Error", "Failed to register window class!", MB_OK);
		return false;
	}

	m_hAppWindow = CreateWindowEx(0, wcex.lpszClassName, m_AppTitle.c_str(), m_dwWindowStyle, 300, 300, m_nClientWidth, m_nClientHeight, NULL, NULL, m_hAppInstance, NULL);

	if(!m_hAppWindow)
	{
		MessageBoxA(NULL, "Error", "Failed to create window!", MB_OK);
		return false;
	}

	ShowWindow(m_hAppWindow, SW_SHOW);

	return true;
}