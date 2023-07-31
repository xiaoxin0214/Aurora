
#include "Aurora.h"

class ExampleLayer :public Aurora::Layer {
public:
	ExampleLayer() :Layer("Example")
	{

	}

	void OnUpdate()override {
		AURORA_TRACE("ExampleLayer::OnUpdate");
	}
	
	void OnEvent(const Aurora::Event& e)override
	{
		AURORA_TRACE("ExampleLayer::OnEvent:{0}",e.ToString());
	}
};
class SandBox :public Aurora::Application {
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Aurora::ImGuiLayer());
	}
};


Aurora::Application* Aurora::CreateApplication()
{
	return new SandBox();
}