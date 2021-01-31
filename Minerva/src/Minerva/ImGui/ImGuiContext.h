#pragma once


namespace Minerva
{
    class Window;

    class ImGuiContext
    {
    public:
        explicit ImGuiContext(Window& window);
        ~ImGuiContext();

        void beginFrame();
        void endFrame();

        // By default, we block events Key, MouseButton, and MouseScroll events when ImGui wants input.
        // The following functions override this behaviour when overrideBlock is true.
        void setOverrideBlockKeyEvents(bool overrideBlock);
        void setOverrideBlockMouseButtonEvents(bool overrideBlock);
        void setOverrideBlockMouseScrollEvents(bool overrideBlock);
        // Overrides blocking of the next MouseButtonRelease event for the corresponding mouse button.
        void setOverrideBlockMouseButtonLeftRelease(bool overrideBlock);
        void setOverrideBlockMouseButtonRightRelease(bool overrideBlock);
        // Reset all event blocking override flags.
        void resetOverrideFlags();
    };

}