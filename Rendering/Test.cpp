#include "Window.h"
#include "Core.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Window w1,
		   w2,
		   w3;

	w1.Create(1280, 720, L"Untitled1");
	w2.Create(1280, 720, L"Untitled2");
	w3.Create(1280, 720, L"Untitled3");
	w1.ShowWindow();
	w2.ShowWindow();
	w3.ShowWindow();

	SwapChain sw(w1);

	while(w1 || w2 || w3)
	{
		w1.ProcessMessage();
		w2.ProcessMessage();
		w3.ProcessMessage();
		sw.Present(0);
	}

	return 0;
}