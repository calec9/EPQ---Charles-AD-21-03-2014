#include <iostream>
#include "windows.h"

using namespace::std;

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
//	virtual ~D3DApp(void);

	virtual bool Init();
	virtual int Run();

	virtual LRESULT __stdcall lpfnWndProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HINSTANCE m_hAppInstance;
	HWND m_hAppWindow;
	UINT m_nClientWidth;
	UINT m_nClientHeight;
	std::string m_AppTitle;
	DWORD m_dwWindowStyle;
	bool bFullScreen;

protected:
	bool InitializeMainwindow();
};