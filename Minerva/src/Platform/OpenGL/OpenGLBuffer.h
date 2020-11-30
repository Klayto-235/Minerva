#pragma once

#include "Minerva/Renderer/Buffer.h"


namespace Minerva
{

    class OpenGLVertexBuffer : public VertexBuffer
    {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t count);
		~OpenGLVertexBuffer();

		void bind() const override;
		void unbind() const override;
	private:
		uint32_t m_rendererID;
    };

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		void bind() const override;
		void unbind() const override;

		uint32_t getCount() const override { return m_count; }
	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};

}