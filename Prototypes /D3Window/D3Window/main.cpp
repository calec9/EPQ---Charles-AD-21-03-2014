//preprocessors
#define WIN32_LEAN_AND_MEAN // excludes some stuff from the windows header for faster compiling

//includes
#include "windows.h"
#include <d3d9.h>
#include <d3dx9.h>
//#include "GRect.h"

//definitions
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

//global variables
HWND g_hwnd;			// handle to application window
int g_nClientWidth = 800;  // window height
int g_nClientHeight = 600; // window width

//D3D global variables
LPDIRECT3D9 d3d; // a long pointer to the direct3D COM (Component Object Model)
LPDIRECT3DDEVICE9 d3ddev; // Direct3D Device - a long pointer to the class that stores information to hardware related stuff pertaining to graphics
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL; // the vertex buffer we will use to copy our vertices to. 
LPD3DXFONT g_Font = NULL; //font object
D3DXFONT_DESC FontDesc;
RECT FontPosition;

//function prototypes
bool g_bInitMainWindow(HINSTANCE hInstance); // global function that will initialize the main window. must return true
LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // message procedure, will handle messages sent to the window and dispatch them 
void initializeD3D(HWND hWnd);
void initializeGraphics(void);
void renderFrame(void);
void cleanD3D(void);
void draw(void);

struct CUSTOMVERTEX // this will be our vertex structure for drawing triangles 
{
    FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
    DWORD color;    // from the D3DFVF_DIFFUSE flag
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) // entry point
{
	if(!g_bInitMainWindow(hInstance)) // the entry point has the hinstance to the window as a parameter. we will pass it to our function to start with creating the window
	{
		MessageBoxA(NULL, "Failed to create window.", "ERROR", MB_OK);
		return -1;
	}
	MSG msg = { 0 }; // message stack. null at first execution 
	 
	RECT r = { 0, 0, 50, 50 }; // a rectangle. NOT D3D

	while(msg.message != WM_QUIT) // here we will process all messages passed to the window (keystrokes, keyboard stuff, etc..) and dispatch them to the MsgProc function. 
	{
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			renderFrame();
		}
	}
	cleanD3D();
	return static_cast<int>(msg.wParam); //The error code is stored in the wParam member of our msg struct;
}

/* 
	*This is the standard "window creation" prolog. It's not very interesting, and will not vary very much from application to application. 
	Explanations will be detailed (maybe...)
*/
bool g_bInitMainWindow(HINSTANCE hInstance) 
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = MsgProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	wcex.lpszClassName = "DX";
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wcex))
		return false;

	RECT r = { 0, 0, g_nClientWidth, g_nClientHeight };
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&r, style, false);
	int width = r.right - r.left;
	int height = r.bottom - r.top;	
	int x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
	int y = GetSystemMetrics(SM_CYSCREEN)/2 - height/2;


	g_hwnd = CreateWindow("DX", "Window title", style, x, y, width, height, NULL, NULL, hInstance, NULL);
	if(!g_hwnd)
		return false;

	ShowWindow(g_hwnd, SW_SHOW);

	    // set up and initialize Direct3D
    initializeD3D(g_hwnd);

	return true;
}

LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		return 0;
	}
	//Calls the default window procedure to provide default processing for any window messages that an application does not process.
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



void initializeD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION); // creates the direct 3d interface. the return value of this call will be the address of the interface that will be stored in the pointer created earlier. 
											// the parameter is the version of the Direct 3D SDK running on the machine. 

	D3DPRESENT_PARAMETERS d3dpp; // a struct whos members contain info about the graphics device. 

	ZeroMemory(&d3dpp, sizeof(d3dpp)); // init all members of d3dpp to zero.
	d3dpp.Windowed = TRUE; // self explanatory. (must concorde with the values set inside the WNDCLASSEX)
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // this relates to the swapping of the backbuffers - this is the swap chain we will use: the swap chain that gives us the best speed possible
	d3dpp.hDeviceWindow = hWnd; // pass the handle to the window to the struct defined above

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev); // this function is a member of the d3d class
																													   // it will create a graphical device interface (class!) used to handle all the graphics we need
   /* Parameters info 
	 * UINT Adapter: stores a value indicating which adapter to use (or video card) - we will use the default (and only one, usually)
	 * D3DDEVTYPE DeviceType: HAL = Hardware Abstraction Layer, which means that Direct3D should use hardware to process graphics, but in the case that it can't, it will use software (a bit of a strange one, this..)
	 * HWND hFocusWindow: just a handle to our window (the same one that we will use all the time)
	 * DWORD BehaviourFlags: there are many parameters we can pass here, but the main ones used are D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DCREATE_MIXED_VERTEXPROCESSING and D3DCREATE_HARDWARE_VERTEXPROCESSING
	   all are fairly self explanatory. in our case 3D calculations will be done by software. (we can use mixed, or just hardware, as stated above) these can be experimented with!
	 * D3DPRESENT_PARAMETERS *pPresentationParameters: just a pointer to the d3dpp we created earlier. so we'll just pass that through.
	 * IDirect3DDevice9 **ppReturnedDeviceInterface: this is a pointer to a pointer to the graphics defice interface declared globally earlier. 
    */

	initializeGraphics();
	//draw();
}


void initializeGraphics(void)
{
	//CUSTOMVERTEX vertices[] = // this is an array of vertices, it will create a triangle from 3 base and a peak one. easy enough to understand the parameters passed via the struct
	//{
	//	{ 0.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
 //       { 520.0f, 400.0f, 5.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
 //       { 120.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
	//};

//	CUSTOMVERTEX va[] = { 200.0f, 100.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 22) };

	d3ddev->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, NULL);
	/* Vertex buffer create
	  * UINT Length: the size of the buffer that will be created. we're using three of our custem vertex format, so it's 3* the sizeof(CUSTOMVERTEX)
	  * DWORD Usage: there can be special ways in which directx has to handle vertices, but no in the case of this program. 
	  * DWORD FVF: here we use the struct of our own Flexible Vertex Format, so it knows which format it's using to create the buffer
	  * D3DPOOL Pool: here we say where to create the vertex buffer and how, there are various flags we can use, of such are creating the buffer in RAM, as opposed to VRAM, which we are using with the D3DPOOL_MANAGED. 
	  * LPDIRECT3DVERTEXBUFFER9 ppVertexBuffer: this is simply the a pointer to the interface we are creating. we filled it in as blank at declaration because this function will fill it in for us. 
	  * HANDLE* pSharedHandle: "Reserved. Set this parameter to NUL" - Microsoft. (not much can be said..)
	*/

	// now the vertex buffer is created and we have loaded the vertices into it. 

	VOID *pVoid; // this is a pointer to a pointer to a void, which means it points to no particular datatype, so it can be converted without issues. 
				// This pointer gets filled with the location of the memory to contain our vertices.
	v_buffer->Lock(0, 0, (void**)&pVoid, 0); // lock vertex buffer memory location, so we can write to it without it warping.
	//memcpy(pVoid, vertices, sizeof(vertices));  // copy from the source to the destination the vertices (the vertex buffer)
	v_buffer->Unlock(); // unlock
}

void renderFrame(void)
{
	// clear the window to deep blue.

	d3ddev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	// the first two parameters of this function are to clear a specific region.  
	// D3DCLEAR_TARGET means we clear the back buffer. 

	d3ddev->BeginScene(); // render frame prolog 
	// this function NEEDS to be called when writing to the memory of the buffer, this function will LOCK the VRAM, granting us exclusive acess to it. 
	// as memory is dynamic, we need to have a region ready for writing, so it doesn't appear elsewhere. 

		d3ddev->SetFVF(CUSTOMFVF); // telling dx which vertex code we're using - as we could have many. 
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX)); // which vertex we are drawing buffer from.
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		  FontPosition.top = 0;         //position
			g_Font->DrawText(NULL,
                   "Direct3D Tutorial 09: Text With D3DXFont",
                   -1,
                   &FontPosition,
                   DT_CENTER,
                   0xffffffff); //draw text
			if(g_Font != NULL)
			{
				g_Font->Release();
				g_Font = NULL;
			}
	d3ddev->EndScene(); // render frame epilog
	// this will unlock the VRAM 

	d3ddev->Present(NULL, NULL, NULL, NULL); // display the frame. 
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	 v_buffer->Release();    // close and release the vertex buffer
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3Dcrea
}