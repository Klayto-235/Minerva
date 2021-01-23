#include "mnpch.h"
#include "Minerva/Renderer/RenderAPI.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"


namespace Minerva
{

	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	Scope<RenderAPI> RenderAPI::create()
	{
		switch (s_API)
		{
		case RenderAPI::API::OpenGL: return createScope<OpenGLRenderAPI>();
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Unknown RenderAPI::API.");
			return nullptr;
		}
	}

}