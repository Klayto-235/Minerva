#pragma once

#include <cstdint>


namespace Minerva
{

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static VertexBuffer* create(float* vertices, uint32_t count);
	};

	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		virtual ~IndexBuffer() = default;
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static IndexBuffer* create(uint32_t* indices, uint32_t count);
	};

}