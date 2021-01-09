#pragma once

#include "Minerva/Renderer/Buffer.h"


namespace Minerva
{

    class OpenGLVertexBuffer : public VertexBuffer
    {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t count);
		~OpenGLVertexBuffer();

		void bind() const override;
		void unbind() const override;

		void setData(const void* data, uint32_t size) override;

		virtual const BufferLayout& getLayout() const override { return m_layout; }
		virtual void setLayout(const BufferLayout& layout) override { m_layout = layout; }
	private:
		uint32_t m_renderID;
		BufferLayout m_layout;
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
		uint32_t m_renderID;
		uint32_t m_count;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void bind() const override;
		void unbind() const override;

		void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const { return m_vertexBuffers; }
		const Ref<IndexBuffer>& getIndexBuffer() const { return m_indexBuffer; }
	private:
		uint32_t m_renderID;
		std::vector<Ref<VertexBuffer>> m_vertexBuffers;
		Ref<IndexBuffer> m_indexBuffer;
		uint32_t m_nAttributes = 0;
	};

}