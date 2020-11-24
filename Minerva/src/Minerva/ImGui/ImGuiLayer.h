#pragma once

#include "Minerva/Layer.h"

namespace Minerva
{

    class MINERVA_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        bool onEvent(const Event& event) override;
    private:
        float m_time = 0.0f;
    };

}