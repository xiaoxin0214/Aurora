#include "pch.h"
#include "LayerStack.h"
namespace Aurora {
	LayerStack::LayerStack()
	{
		m_insertIter = m_lyrs.begin();
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
		m_insertIter=m_lyrs.emplace(m_insertIter, lyr);
	}

	void LayerStack::PopLayer(Layer* lyr)
	{
		auto iter = std::find(m_lyrs.begin(), m_lyrs.end(), lyr);
		if (iter != m_lyrs.end())
		{
			m_lyrs.erase(iter);
			m_insertIter--;
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