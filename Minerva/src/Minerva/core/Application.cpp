#include "mnpch.h"
#include "Minerva/core/Application.h"
#include "Minerva/Renderer/Renderer.h"
#include "Minerva/Events/WindowEvent.h"

#include <GLFW/glfw3.h> // TEMPORARY (timestep)


namespace Minerva
{

#ifdef MN_ENABLE_ASSERTS
	bool g_lockWindows = false;
#endif

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		MN_PROFILE_FUNCTION();
		MN_CORE_ASSERT(!s_instance, "Application already exists.");
		s_instance = this;

		MN_CORE_INFO("Initialising window system.");
		Minerva::Window::init();
	}

	Application::~Application()
	{
		MN_PROFILE_FUNCTION();

		disableImGui();
		m_windows.clear();

		MN_CORE_INFO("Terminating window system.");
		Minerva::Window::terminate();

		s_instance = nullptr;
	}

	void Application::run()
	{
		MN_PROFILE_FUNCTION();

		m_running = true;
		m_lastFrameTime = static_cast<float>(glfwGetTime());

		while (m_running)
		{
			MN_PROFILE_SCOPE("Run loop iteration");

			for (auto& event : m_eventBuffer)
				onEvent(*event);
			m_eventBuffer.clear();
			onUpdate();

			const float currentTime = static_cast<float>(glfwGetTime());
			const float deltaTime = currentTime - m_lastFrameTime;
			m_lastFrameTime = currentTime;

			Window::pollEvents();

			if (m_enableImGui)
			{
				m_ImGuiWindow->m_graphicsContext->makeCurrent();
				m_ImGuiContext->beginFrame();

				for (auto& window : m_windows)
				{
					window->m_graphicsContext->makeCurrent();
#ifdef MN_ENABLE_ASSERTS
					g_lockWindows = true;
					window->onUpdate(deltaTime, m_eventBuffer);
					window->onImGuiRender();
					g_lockWindows = false;
#else
					window->onUpdate(deltaTime, m_eventBuffer);
					window->onImGuiRender();
#endif
					if (window.get() != m_ImGuiWindow) // necessary?
						window->m_graphicsContext->swapBuffers();
				}

				m_ImGuiWindow->m_graphicsContext->makeCurrent();
				m_ImGuiContext->endFrame();
				m_ImGuiWindow->m_graphicsContext->swapBuffers(); // necessary?
			}
			else
			{
				for (auto& window : m_windows)
				{
					window->m_graphicsContext->makeCurrent();
#ifdef MN_ENABLE_ASSERTS
					g_lockWindows = true;
					window->onUpdate(deltaTime, m_eventBuffer);
					g_lockWindows = false;
#else
					window->onUpdate(deltaTime, m_eventBuffer);
#endif
					window->m_graphicsContext->swapBuffers();
				}
			}
		}
	}

	Window* Application::createWindow(const WindowProperties& properties)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Cannot create window while windows are locked.");
		m_windows.push_back(Scope<Window>(new Window(properties)));
		return m_windows.back().get();
	}

	void Application::deleteWindow(Window* window)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Cannot delete window while windows are locked.");
		auto it = std::find_if(m_windows.begin(), m_windows.end(),
			[window](const auto& other) { return window == other.get(); });
		MN_CORE_ASSERT(it != m_windows.end(), "Could not find window.");

		if (m_enableImGui && (*it).get() == m_ImGuiWindow) disableImGui();
		m_windows.erase(it);
	}

	void Application::enableImGui(Window* window)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Cannot enable ImGui while windows are locked.");
		MN_CORE_ASSERT(std::find_if(m_windows.begin(), m_windows.end(),
			[window](const auto& other) { return window == other.get(); }) != m_windows.end(), "Window does not exist.");
		if (m_enableImGui) disableImGui();
		m_ImGuiWindow = window;
		m_ImGuiWindow->m_graphicsContext->makeCurrent();
		m_ImGuiContext = createScope<ImGuiContext>(*window);
		m_ImGuiContext->resetOverrideFlags();
		m_enableImGui = true;
	}

	void Application::disableImGui()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Cannot disable ImGui while windows are locked.");
		if (m_enableImGui)
		{
			m_enableImGui = false;
			m_ImGuiWindow->m_graphicsContext->makeCurrent();
			m_ImGuiContext.reset();
			m_ImGuiWindow = nullptr;
		}
	}

	void Application::setImGuiViewportWindowState(bool focused, bool hovered)
	{
		if (m_enableImGui)
		{
			if (focused != m_ImGuiViewportWindowFocused)
			{
				m_ImGuiViewportWindowFocused = focused;
				m_ImGuiContext->setOverrideBlockKeyEvents(focused);
				m_ImGuiContext->setOverrideBlockMouseScrollEvents(focused && hovered);

				if (!focused)
				{
					m_ImGuiContext->setOverrideBlockMouseButtonLeftRelease(false);
					m_ImGuiContext->setOverrideBlockMouseButtonRightRelease(false);
					m_ImGuiWindow->m_data.inputState.onLoseFocus();
				}
			}
			if (hovered != m_ImGuiViewportWindowHovered)
			{
				m_ImGuiViewportWindowHovered = hovered;
				m_ImGuiContext->setOverrideBlockMouseScrollEvents(focused && hovered);
				m_ImGuiContext->setOverrideBlockMouseButtonEvents(hovered);

				if (!hovered)
				{
					if (m_ImGuiWindow->m_data.inputState.isMouseButtonPressed(MouseButton::Left))
						m_ImGuiContext->setOverrideBlockMouseButtonLeftRelease(true);
					if (m_ImGuiWindow->m_data.inputState.isMouseButtonPressed(MouseButton::Right))
						m_ImGuiContext->setOverrideBlockMouseButtonRightRelease(true);
				}
			}
		}
	}

	void Application::onEvent(const Event& event)
	{
		MN_PROFILE_FUNCTION();

		// Currently, only WindowCloseEvent(s) get here.
		deleteWindow(static_cast<const WindowCloseEvent&>(event).getWindow());
		if (m_windows.size() == 0)
			m_running = false;
	}

}