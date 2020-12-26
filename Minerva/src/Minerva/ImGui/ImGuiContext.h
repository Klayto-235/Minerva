#pragma once

#include <imgui.h>


namespace Minerva
{
    class Window;

    class ImGuiContext
    {
    public:
        ImGuiContext(Window& window);
        ~ImGuiContext();

        void beginFrame();
        void endFrame();
    private:
        ::ImGuiContext* m_context;
    };

}