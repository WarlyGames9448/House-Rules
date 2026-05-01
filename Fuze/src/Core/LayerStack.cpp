#include "fuzepch.h"

#include "LayerStack.h"

namespace Fuze {
LayerStack::LayerStack() {
}

LayerStack::~LayerStack() {
    for (Layer* layer : m_Layers) {
        layer->OnDetach();
        delete layer;
    }
}

void LayerStack::PushLayer(Layer* layer) {
    FUZE_PROFILE_FUNCTION();

    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* layer) {
    FUZE_PROFILE_FUNCTION();

    m_Layers.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer) {
    FUZE_PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PopOverlay(Layer* layer) {
    FUZE_PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
        m_Layers.erase(it);
    }
}
}
