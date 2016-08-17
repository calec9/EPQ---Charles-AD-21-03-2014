#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "D3dx9math.h"
#include "d3d9.h"
#include <string>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "D3dx9.lib")

#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX  //Flexible Vertex Format
{
	FLOAT x, y, z, rhw; // from the D3DFVF_XYZRHW flag
	DWORD color; //  from the D3DFVF_DIFFUSE flag
};

class Direct3D
{
public:
	Direct3D(HINSTANCE hInstance);
	virtual ~Direct3D();

	// main app loop
	int Run();
	
	// framework
	virtual bool Init(); 
	virtual void Update(float dt) = 0; // = 0 = MUST be overriden, so no need to define
	virtual void Render(float dt) = 0; // ditto.
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void EndScene();
	void ViewSetup();
	void BeginScene();
	void drawInfo();
	void drawMenu();
	void statistics(int rotID);
	int getFPS();

protected: // inheritant classes have access to there.
	// win32 attributes
	HWND m_hwndAppWindow; // hwindow, handle to our window
	HINSTANCE m_hInstance; // our instance to the window
	UINT m_nClientWidth;
	UINT m_nClientHeight;
	const char* m_chAppTitle;
	DWORD m_dwWindowStyle;
	LPD3DXFONT g_Font; //font object

	//dx runtime variables
	IDirect3D9* m_pDirect3D;
	IDirect3DDevice9 *m_pDevice;
	D3DPRESENT_PARAMETERS m_pD3dpp;
	D3DDISPLAYMODE			m_Mode;
	D3DDEVTYPE				m_DevType;			



protected:
	bool InitializeWindow();
	bool InitializeD3D();
};