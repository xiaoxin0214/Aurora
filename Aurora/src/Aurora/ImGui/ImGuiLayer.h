#pragma once
#include "Aurora/Core.h"
#include "Aurora/Layer.h"
namespace Aurora
{
	class AURORA_API ImGuiLayer:public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnDetach()override;
		void OnAttach()override;
		void OnUpdate()override;
		void OnEvent(const Event& e)override;
	private:
		float m_time;
	};
}