#pragma once
#include "Core.h"
#include "Layer.h"
namespace Aurora
{
	class AURORA_API LayerStack {
	public:
		LayerStack();
		~LayerStack();
	public:
		void PushLayer(Layer* lyr);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* lyr);
		void PopOverlay(Layer* overlay);
		inline std::vector<Layer*>::iterator begin() {
			return m_lyrs.begin();
		}

		inline std::vector<Layer*>::iterator end()
		{
			return m_lyrs.end();
		}

	private:
		std::vector<Layer*>            m_lyrs;
		std::int32_t                   m_insertIndex;
	};
}