#include "mnpch.h"
#include "Minerva/ImGui/ImGuiContext.h"
#include "Minerva/core/Window.h"

#include <imgui.h>
#include "Minerva/ImGui/imgui_impl_opengl3.h"
#include "Minerva/ImGui/imgui_impl_glfw.h"

#include <GLFW/glfw3.h>


namespace Minerva
{
	extern bool g_ImGuiOverrideBlockKeyEvents;
	extern bool g_ImGuiOverrideBlockMouseButtonEvents;
	extern bool g_ImGuiOverrideBlockMouseScrollEvents;
	extern bool g_ImGuiOverrideBlockMouseButtonLeftRelease;
	extern bool g_ImGuiOverrideBlockMouseButtonRightRelease;

	ImGuiContext::ImGuiContext(Window& window)
	{
		MN_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* windowHandle = static_cast<GLFWwindow*>(window.getNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(windowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	ImGuiContext::~ImGuiContext()
	{
		MN_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiContext::beginFrame()
	{
		MN_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiContext::endFrame()
	{
		MN_PROFILE_FUNCTION();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiContext::setOverrideBlockKeyEvents(bool overrideBlock)
	{
		g_ImGuiOverrideBlockKeyEvents = overrideBlock;
	}

	void ImGuiContext::setOverrideBlockMouseButtonEvents(bool overrideBlock)
	{
		g_ImGuiOverrideBlockMouseButtonEvents = overrideBlock;
	}

	void ImGuiContext::setOverrideBlockMouseScrollEvents(bool overrideBlock)
	{
		g_ImGuiOverrideBlockMouseScrollEvents = overrideBlock;
	}

	void ImGuiContext::setOverrideBlockMouseButtonLeftRelease(bool overrideBlock)
	{
		g_ImGuiOverrideBlockMouseButtonLeftRelease = overrideBlock;
	}

	void ImGuiContext::setOverrideBlockMouseButtonRightRelease(bool overrideBlock)
	{
		g_ImGuiOverrideBlockMouseButtonRightRelease = overrideBlock;
	}

	void ImGuiContext::resetOverrideFlags()
	{
		g_ImGuiOverrideBlockKeyEvents = false;
		g_ImGuiOverrideBlockMouseButtonEvents = false;
		g_ImGuiOverrideBlockMouseScrollEvents = false;
		g_ImGuiOverrideBlockMouseButtonLeftRelease = false;
		g_ImGuiOverrideBlockMouseButtonRightRelease = false;
	}

}