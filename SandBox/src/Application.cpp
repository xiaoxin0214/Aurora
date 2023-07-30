
#include "Aurora.h"

class SandBox :public Aurora::Application {
	
};


Aurora::Application* Aurora::CreateApplication()
{
	return new SandBox();
}