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

		void BlockEvents(bool block)
		{
			m_blockEvents = block;
		}
		void OnDetach()override;
		void OnAttach()override;

		void Begin();
		void OnImGuiRender()override;
		void OnEvent(Event&e)override;
		void End();
	private:
		float m_time;
		bool  m_blockEvents;
	};
}