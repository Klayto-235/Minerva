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
        void onImGuiRender() override;

        void begin();
        void end();
    private:
        float m_time = 0.0f;
    };

}