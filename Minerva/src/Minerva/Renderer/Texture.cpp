#include "mnpch.h"
#include "Minerva/Renderer/Texture.h"
#include "Minerva/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Minerva
{

	Ref<Texture2D> Texture2D::create(const std::string& filePath)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLTexture2D>(filePath);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "OpenGLTexture2D::create: RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "OpenGLTexture2D::create: Unknown RenderAPI::API.");
			return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLTexture2D>(width, height);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "OpenGLTexture2D::create: RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "OpenGLTexture2D::create: Unknown RenderAPI::API.");
			return nullptr;
		}
	}

}