#pragma once

#include "Minerva/core/core.h"
#include "Minerva/core/Layer.h"

#include <vector>


namespace Minerva
{

	class LayerStack
	{
	public:
		LayerStack() = default;
		LayerStack(const LayerStack&) = delete;
		LayerStack& operator=(const LayerStack&) = delete;
		~LayerStack() { clear(); }

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		size_t removeLayer(Layer* layer);
		size_t removeOverlay(Layer* overlay);
		Layer* popLayer();
		Layer* popOverlay();
		void clear();

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }
	private:
		std::vector<Layer*> m_layers;
		uint32_t m_nLayers = 0;
	};

}