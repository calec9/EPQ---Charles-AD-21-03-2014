#include "D3DApp.h"

class winmain : public D3DApp
{
public:	
	winmain(HINSTANCE hInstance);
	bool Init() override;
	void Update(void);
	void Render(void);
};

winmain::winmain(HINSTANCE hInstance):D3DApp(hInstance)
{

}

bool winmain::Init()
{
	return D3DApp::InitializeMainwindow();
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MessageBoxA(NULL, "HELO", "HELO", MB_OK);
	winmain *wmain = new winmain(hInstance);
	if(!wmain->Init())
		return -1;
	else
		return (wmain->Run());
}