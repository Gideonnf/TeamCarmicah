#include <iostream>
#include "CarmicahCore.h"

int main(int argc, char** argv)
{
	_CRT_UNUSED(argc);
	_CRT_UNUSED(argv);
	Carmicah::Application* app = new Carmicah::Application();
	app->run();

	delete app;

	return 0;
}