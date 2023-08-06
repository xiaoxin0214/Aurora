#include "pch.h"
#include "LayerStack.h"
namespace Aurora {
	LayerStack::LayerStack()
	{
		m_insertIndex = 0;
	}

	LayerStack::~LayerStack()
	{
		for (auto& lyr : m_lyrs)
		{
			delete lyr;
		}

		m_lyrs.clear();
	}

	void LayerStack::PushLayer(Layer* lyr)
	{
		m_lyrs.emplace(m_lyrs.begin()+m_insertIndex, lyr);
		m_insertIndex++;
	}

	void LayerStack::PopLayer(Layer* lyr)
	{
		auto iter = std::find(m_lyrs.begin(), m_lyrs.end(), lyr);
		if (iter != m_lyrs.end())
		{
			m_lyrs.erase(iter);
			m_insertIndex--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_lyrs.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iter = std::find(m_lyrs.begin(), m_lyrs.end(), overlay);
		if (iter != m_lyrs.end())
		{
			m_lyrs.erase(iter);
		}
	}
}