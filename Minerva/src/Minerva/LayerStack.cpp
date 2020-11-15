#include "mnpch.h"
#include "LayerStack.h"


namespace Minerva
{

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_nLayers, layer);
		++m_nLayers;
		layer->onAttach();
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->onAttach();
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_nLayers, layer);
		if (it != m_layers.end())
		{
			layer->onDetach();
			m_layers.erase(it);
			--m_nLayers;
		}
		MN_CORE_ASSERT(it != m_layers.end(), "LayerStack::popLayer ({0}): could not find Layer {1}.",
			static_cast<void*>(this), static_cast<void*>(layer));
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin() + m_nLayers, m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			overlay->onDetach();
			m_layers.erase(it);
		}
		MN_CORE_ASSERT(it != m_layers.end(), "LayerStack::popOverlay ({0}): could not find Layer {1}.",
			static_cast<void*>(this), static_cast<void*>(overlay));
	}

}