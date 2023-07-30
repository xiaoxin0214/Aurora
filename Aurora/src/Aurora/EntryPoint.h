#pragma once
#ifdef AURORA_PLATFORM_WINDOWS
extern Aurora::Application* Aurora::CreateApplication();
int main(int argc, char** argv)
{
	Aurora::Log::Init();
	auto pApp = Aurora::CreateApplication();
	pApp->Run();
	delete pApp;
}
#endif