#pragma once

#include "Minerva/core/Layer.h"

namespace Minerva
{

    class  ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer()
            : Layer("ImGui Layer") {}

        void onAttach() override;
        void onDetach() override;
        void onImGuiRender() override;

        void begin();
        void end();
    };

}