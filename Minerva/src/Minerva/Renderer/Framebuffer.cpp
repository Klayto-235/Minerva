#include "mnpch.h"
#include "Minerva/Renderer/Framebuffer.h"
#include "Minerva/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"


namespace Minerva
{

	Ref<Framebuffer> Framebuffer::create(const FramebufferProperties& properties)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLFramebuffer>(properties);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Unknown RenderAPI::API.");
			return nullptr;
		}
	}

}
