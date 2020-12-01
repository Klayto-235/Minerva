#include "mnpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Minerva {

	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, count);
		case RendererAPI::None:
			MN_CORE_ASSERT(false, "IndexBuffer::create: RendererAPI::None is currently not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "IndexBuffer::create: unknown RendererAPI.");
			return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
		case RendererAPI::None:
			MN_CORE_ASSERT(false, "IndexBuffer::create: RendererAPI::None is currently not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "IndexBuffer::create: unknown RendererAPI.");
			return nullptr;
		}
	}

	VertexArray* VertexArray::create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		case RendererAPI::None:
			MN_CORE_ASSERT(false, "VertexArray::create: RendererAPI::None is currently not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "IndexBuffer::create: unknown RendererAPI.");
			return nullptr;
		}
	}
}