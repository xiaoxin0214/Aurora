#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
namespace Aurora
{
	class AURORA_API Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(const Event& e) {};
		inline const std::string& GetLayerName()const {
			return m_debugName;
		}

	private:
		std::string      m_debugName;
	};
}