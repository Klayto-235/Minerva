#pragma once

#include "Minerva/Window.h"
#include "Minerva/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Minerva
{
	class WindowsWindow : public Window
	{
	public:
		explicit WindowsWindow(const WindowProperties& properties);
		~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;
		void setVSync(bool enabled) override;
		bool isVSync() const override;
		const EventBuffer& getEventBuffer() const override;
		void* getNativeWindow() override { return static_cast<void*>(m_window); }

		bool isKeyPressed(Key key) const override;
		bool isMouseButtonPressed(MouseButton button) const override;
		std::pair<float, float> getMousePosition() const override;
		float getMouseX() const override;
		float getMouseY() const override;
	private:
		GLFWwindow* m_window;
		std::unique_ptr<GraphicsContext> m_context;

		struct WindowData
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool VSync;
			EventBuffer eventBuffer;
		};

		WindowData m_data;
	};

}