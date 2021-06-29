#include "mnpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>


namespace Minerva
{

#pragma region /////////////// VertexBuffer ///////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		MN_PROFILE_FUNCTION();

		glGenBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t count)
	{
		MN_PROFILE_FUNCTION();

		glGenBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		{
			MN_PROFILE_SCOPE("Upload data - OpenGLVertexBuffer::OpenGLVertexBuffer");

			glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), vertices, GL_STATIC_DRAW);
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		MN_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_renderID);
	}

	void OpenGLVertexBuffer::bind() const
	{
		MN_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
	}

	void OpenGLVertexBuffer::unbind() const
	{
		MN_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

#pragma endregion

#pragma region /////////////// IndexBuffer ///////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		MN_PROFILE_FUNCTION();

		glGenBuffers(1, &m_renderID);
		// GL_ELEMENT_ARRAY_BUFFER requires a VA to be bound, GL_ARRAY_BUFFER allows loading data without VA.
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		MN_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_renderID);
	}

	void OpenGLIndexBuffer::bind() const
	{
		MN_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		MN_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

#pragma endregion

#pragma region /////////////// VertexArray ///////////////

	static constexpr GLenum shaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		default:
			MN_CORE_ASSERT(false, "Unknown ShaderDataType.");
			return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		MN_PROFILE_FUNCTION();

		glGenVertexArrays(1, &m_renderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		MN_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_renderID);
	}

	void OpenGLVertexArray::bind() const
	{
		MN_PROFILE_FUNCTION();

		glBindVertexArray(m_renderID);
	}

	void OpenGLVertexArray::unbind() const
	{
		MN_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex buffer layout has no elements.");

		glBindVertexArray(m_renderID);
		vertexBuffer->bind();

		const auto& layout = vertexBuffer->getLayout();
		uint32_t stride = layout.getStride();
		for (const auto& element : layout)
		{
			const auto componentCount = shaderDataTypeComponentCount(element.type);
			const auto componentType = shaderDataTypeToOpenGLType(element.type);
			const auto normalised = element.normalised ? GL_TRUE : GL_FALSE;

			uint8_t iterationCount = 1;
			if (element.type == ShaderDataType::Mat3 || element.type == ShaderDataType::Mat4)
				iterationCount = componentCount;

			for (uint8_t i = 0; i < iterationCount; ++i)
			{
				glEnableVertexAttribArray(m_nAttributes);
				glVertexAttribPointer(m_nAttributes,
					componentCount,
					componentType,
					normalised,
					stride,
					reinterpret_cast<const void*>((uint64_t)element.offset + (uint64_t)(4*componentCount*i)));
				++m_nAttributes;
			}
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		MN_PROFILE_FUNCTION();

		glBindVertexArray(m_renderID);
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;
	}

#pragma endregion

}