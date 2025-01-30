#include <Windows.h>
#include <iostream>
#include "CarmicahCore.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CRT_UNUSED(hInstance);
	_CRT_UNUSED(hPrevInstance);
	_CRT_UNUSED(lpCmdLine);
	_CRT_UNUSED(nCmdShow);
	Carmicah::Application* app = new Carmicah::Application();
	app->run();

	delete app;

	return 0;
}

//int main()
//{
//	Carmicah::Application* app = new Carmicah::Application();
//	app->run();
//
//	delete app;
//
//	return 0;
//}