#include <iostream>
#include "windows.h"
#include "windowsx.h"
#include <d3dx9.h>
#include <d3d9.h>

// include the Direct3D Library file
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

// defines
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


// namespaces we want
using namespace::std;

// global declarations
TCHAR szClassTitle[] = "Wherever I may Roam";
LPDIRECT3D9 d3d; // pointer to our directX interface //    OR CAN BE LPDIRECT3D9 
LPDIRECT3DDEVICE9 d3ddev; // pointer to the device class // OR LPDIRECT3DDEVICE9
LPDIRECT3DVERTEXBUFFER9 v_buffer; //  the pointer to the vertex buffer interface we're making
LPD3DXFONT g_Font = NULL; //font object

// function prototypes
WNDPROC lpfnWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPAram); // WindowProcedure prototype
void initD3D(HWND hwnd); // set up and init direct3D
void render_frame(void); // renders a single frame ((void) means NO args
void cleanD3D(void);     // close direct3D and release memory ((ditto))
void init_graphics(void); 

//structures D9
struct CUSTOMVERTEX  //Flexible Vertex Format
{
	FLOAT x, y, z, rhw; // from the D3DFVF_XYZRHW flag
	DWORD color; //  from the D3DFVF_DIFFUSE flag
};

void DrawHello()
{
	D3DXFONT_DESC FontDesc = {24,
                          0,
                          400,
                          0,
                          false,
                          DEFAULT_CHARSET,
                          OUT_TT_PRECIS,
                          CLIP_DEFAULT_PRECIS,
                          DEFAULT_PITCH,
                          "Arial"};
	RECT FontPosition;
	FontPosition.top = 0;
	FontPosition.left = 0;
	FontPosition.right =800;
	FontPosition.bottom =600;
	D3DXCreateFontIndirect(d3ddev,&FontDesc,&g_Font);
	  FontPosition.top = 0;         //position
	 
  g_Font->DrawText(NULL,
                   "Welcome!",
                   -1,
                   &FontPosition,
                   DT_CENTER,
                   0xffffffff); //draw text

  FontPosition.top = 100;       //position
  g_Font->DrawText(NULL,
                   "Text Sample Using D3DXFont",
                   -1,
                   &FontPosition,
                   DT_CENTER,
                   0xffff0000); //draw text
  if(g_Font != NULL)
 {
 g_Font->Release(); //release font
 g_Font = NULL;
 }
}

int main() 
{
	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.lpfnWndProc = (WNDPROC)lpfnWndProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.hIcon = LoadCursor(0,IDC_ARROW);
	windowClass.hCursor =  LoadCursor(0,IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW; // won't be visible as window for a second or two before the game starts
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = szClassTitle;
	windowClass.hIconSm = LoadIcon(0,IDI_APPLICATION);
	if(RegisterClassEx(&windowClass)) 
	{
		HWND hWindow = CreateWindowEx(0, szClassTitle, szClassTitle, WS_EX_TOPMOST | WS_POPUP,0, 0,SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, GetModuleHandle(0), 0 ) ;

		if(hWindow)
		{
			ShowWindow(hWindow,SW_SHOWDEFAULT); 

			initD3D(hWindow);
			init_graphics();
			render_frame();

			MSG msg; // struct
			while(TRUE) 
			{
				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // as opposed to GetMessage()
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if(msg.message == WM_QUIT) 
					break;
			}
		}
	}

	return 0;
}

WNDPROC lpfnWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPAram)
{
	switch(uMsg)
	{
		case WM_DESTROY:
			printf("\n----- destroying window -----\n");
			PostQuitMessage(0);
			return 0L;
		case WM_LBUTTONDOWN:
			std::cout << "\nmouse left button down at (" << LOWORD(lPAram) << ',' << HIWORD(lPAram) << ")\n" ;
		default:
            std::cout << '.' ;
            return (WNDPROC)DefWindowProcW( hwnd, uMsg, wParam, lPAram ) ;
	}
}

void initD3D(HWND hwnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION); // create D3D interface

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp)); // clear out structure for use (optional line of code?)
	d3dpp.Windowed = true; // not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames (makes sense..)
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer

	// create a device class using the info stored in the d3dpp.
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

}

void render_frame(void) // function used to render a single frame
{
	//clear window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	//begin 3d scene
	d3ddev->BeginScene();
	
		// select which vertex format we are using
        d3ddev->SetFVF(CUSTOMFVF);

        // select the vertex buffer to display
        d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

        // copy the vertex buffer to the back buffer
        d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		DrawHello();

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	v_buffer->Release();
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
}

void init_graphics(void) 
{
	CUSTOMVERTEX OurVertices[] =
	{
		{320.0f, 50.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255),},
		{520.0f, 400.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255),},
		{120.0f, 400.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0),},
	};
	d3ddev->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
                           0,
                           CUSTOMFVF,
                           D3DPOOL_MANAGED,
                           &v_buffer,
                           NULL);
	VOID* pVoid; // This pointer gets filled with the location of the memory to contain our vertices
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, OurVertices, sizeof(OurVertices));
	v_buffer->Unlock();
}