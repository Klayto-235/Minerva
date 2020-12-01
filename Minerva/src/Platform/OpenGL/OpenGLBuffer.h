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

		virtual const BufferLayout& getLayout() const override { return m_layout; }
		virtual void setLayout(const BufferLayout& layout) override { m_layout = layout; }
	private:
		uint32_t m_rendererID;
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
		uint32_t m_rendererID;
		uint32_t m_count;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void bind() const override;
		void unbind() const override;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return m_vertexBuffers; }
		const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_indexBuffer; }
	private:
		uint32_t m_rendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};

}