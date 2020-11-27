#include "mnpch.h"
#include "ImGuiLayer.h"
#include "Minerva/Application.h"
#include "Minerva/Events/WindowEvent.h"
#include "Minerva/Events/KeyEvent.h"
#include "Minerva/Events/MouseEvent.h"

#include "imgui.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"

// TEMPORARY
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Minerva
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::onAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		const Application& app = Application::get();
		io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()),
			static_cast<float>(app.getWindow().getHeight()));

		float time = static_cast<float>(glfwGetTime());
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImGuiLayer::onEvent(const Event& event)
	{
		ImGuiIO& io = ImGui::GetIO();

		switch (event.getEventType())
		{
		case EventType::KeyChar:
		{
			int keyCode = static_cast<const KeyCharEvent&>(event).getKeyCode();
			if (keyCode > 0 && keyCode < 0x10000)
				io.AddInputCharacter(static_cast<unsigned short>(keyCode));
			break;
		}
		case EventType::KeyPress:
			io.KeysDown[static_cast<const KeyPressEvent&>(event).getKeyCode()] = true;
			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
			break;

		case EventType::KeyRelease:
			io.KeysDown[static_cast<const KeyReleaseEvent&>(event).getKeyCode()] = false;
			break;

		case EventType::MouseButtonPress:
			io.MouseDown[static_cast<const MouseButtonPressEvent&>(event).getMouseButton()] = true;
			break;

		case EventType::MouseButtonRelease:
			io.MouseDown[static_cast<const MouseButtonReleaseEvent&>(event).getMouseButton()] = false;
			break;

		case EventType::MouseMove:
			io.MousePos = ImVec2(static_cast<const MouseMoveEvent&>(event).getX(),
				static_cast<const MouseMoveEvent&>(event).getY());
			break;

		case EventType::MouseScroll:
			io.MouseWheelH += static_cast<const MouseScrollEvent&>(event).getXOffset();
			io.MouseWheel += static_cast<const MouseScrollEvent&>(event).getYOffset();
			break;

		case EventType::WindowResize:
			io.DisplaySize = ImVec2(static_cast<float>(static_cast<const WindowResizeEvent&>(event).getWidth()),
				static_cast<float>(static_cast<const WindowResizeEvent&>(event).getHeight()));
			io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
			glViewport(0, 0, static_cast<const WindowResizeEvent&>(event).getWidth(),
				static_cast<const WindowResizeEvent&>(event).getHeight());
			break;
		}

		return false;
	}

}