#include "mnpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Minerva
{

	Ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "OpenGLTexture2D::create: RendererAPI::None is currently not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "OpenGLTexture2D::create: Unknown RendererAPI.");
			return nullptr;
		}
	}

}