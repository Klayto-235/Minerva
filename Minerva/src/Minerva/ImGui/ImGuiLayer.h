#pragma once

#include "Minerva/Layer.h"

namespace Minerva
{

    class  ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;
        void onImGuiRender() override;

        void begin();
        void end();
    };

}