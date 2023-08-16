#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Core/Layer.h"
namespace Aurora
{
	class AURORA_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnDetach()override;
		void OnAttach()override;

		void Begin();
		void OnImGuiRender()override;
		void End();
	private:
		float m_time;
	};
}