#include "mnpch.h"
#include "Minerva/Renderer/Buffer.h"
#include "Minerva/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Minerva {

	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t count)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLVertexBuffer>(vertices, count);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Unknown RenderAPI::API.");
			return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLVertexBuffer>(size);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Unknown RenderAPI::API.");
			return nullptr;
		}
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLIndexBuffer>(indices, count);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Unknown RenderAPI::API.");
			return nullptr;
		}
	}

	Ref<VertexArray> VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLVertexArray>();
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Unknown RenderAPI::API.");
			return nullptr;
		}
	}
}