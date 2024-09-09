#include <Carmicah.h>

class mainApp : public Carmicah::Application
{
public:
	mainApp()
	{

	}

	~mainApp()
	{

	}
};

int main()
{
	mainApp* app = new mainApp();
	app->run();

	delete app;

	return 0;
}