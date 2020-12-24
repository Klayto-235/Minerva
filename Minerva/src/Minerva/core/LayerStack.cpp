#include "mnpch.h"
#include "Minerva/core/LayerStack.h"


namespace Minerva
{

	void LayerStack::pushLayer(Layer* layer)
	{
		m_layers.insert(m_layers.begin() + m_nLayers, layer);
		++m_nLayers;
		layer->onAttach();
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		m_layers.push_back(overlay);
		overlay->onAttach();
	}

	int LayerStack::removeLayer(Layer* layer)
	{
		auto it = m_layers.begin();
		int layerCount = m_layers.size();
		while ((it = std::find(it, m_layers.begin() + m_nLayers, layer)) != m_layers.begin() + m_nLayers)
		{
			layer->onDetach();
			it = m_layers.erase(it);
			--m_nLayers;
		}
		return layerCount - m_layers.size();
	}

	int LayerStack::removeOverlay(Layer* overlay)
	{
		auto it = m_layers.begin() + m_nLayers;
		int layerCount = m_layers.size();
		while ((it = std::find(it, m_layers.end(), overlay)) != m_layers.end())
		{
			overlay->onDetach();
			it = m_layers.erase(it);
		}
		return layerCount - m_layers.size();
	}

	Layer* LayerStack::popLayer()
	{
		if (m_nLayers != 0)
		{
			--m_nLayers;
			auto it = m_layers.begin() + m_nLayers; // m_nLayers has been decremented
			Layer* layer = *it;
			layer->onDetach();
			m_layers.erase(it);
			return layer;
		}
		else return nullptr;
	}

	Layer* LayerStack::popOverlay()
	{
		if (m_layers.size() - m_nLayers != 0)
		{
			Layer* overlay = m_layers.back();
			overlay->onDetach();
			m_layers.pop_back();
			return overlay;
		}
		else return nullptr;
	}

	void LayerStack::clear()
	{
		for (auto& layer : m_layers)
			layer->onDetach();
		m_layers.clear();
		m_nLayers = 0;
	}

}