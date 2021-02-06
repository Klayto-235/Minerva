#pragma once

#include "Minerva/core/core.h"
#include "Minerva/core/Log.h"

#include <cstdint>


namespace Minerva
{

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static constexpr uint32_t shaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		default:
			MN_CORE_ASSERT(false, "Unknown data type.");
			return 0;
		}
	}

	static constexpr uint32_t shaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3; // 3 x vec3
		case ShaderDataType::Mat4:		return 4; // 4 x vec4
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		default:
			MN_CORE_ASSERT(false, "Unknown data type.");
			return 0;
		}
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalised;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalised = false)
			: name(name), type(type), size(shaderDataTypeSize(type)), offset(0), normalised(normalised) {}
				
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(elements)
		{
			uint32_t offset = 0;
			for (auto& element : m_elements)
			{
				element.offset = offset;
				offset += element.size;
			}
			m_stride = offset;
		}

		uint32_t getStride() const { return m_stride; }
		const std::vector<BufferElement>& getElements() const { return m_elements; }

		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> create(uint32_t size);
		static Ref<VertexBuffer> create(float* vertices, uint32_t count);
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

		static Ref<IndexBuffer> create(uint32_t* indices, uint32_t count);
	};

	class VertexArray
	{
	public:
		VertexArray() = default;
		virtual ~VertexArray() = default;
		VertexArray(const VertexArray& other) = delete;
		VertexArray& operator=(const VertexArray& other) = delete;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

		static Ref<VertexArray> create();
	};

}