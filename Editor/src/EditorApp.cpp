
#include "EditorLayer.h"
#include "Aurora/Core/EntryPoint.h"

class EditorApp :public Aurora::Application {
public:
	EditorApp():Application("Editor")
	{
		PushLayer(new Aurora::EditorLayer());
	}
};


Aurora::Application* Aurora::CreateApplication()
{
	return new EditorApp();
}