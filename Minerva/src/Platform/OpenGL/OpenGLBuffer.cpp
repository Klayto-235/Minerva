#include "mnpch.h"
#include "OpenGLBuffer.h"
#include "OpenGL_core.h"


namespace Minerva
{

#pragma region /////////////// VertexBuffer ///////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t count)
	{
		GLCALL(glCreateBuffers(1, &m_renderID));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), vertices, GL_STATIC_DRAW));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GLCALL(glDeleteBuffers(1, &m_renderID));
	}

	void OpenGLVertexBuffer::bind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
	}

	void OpenGLVertexBuffer::unbind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

#pragma endregion

#pragma region /////////////// IndexBuffer ///////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		GLCALL(glCreateBuffers(1, &m_renderID));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GLCALL(glDeleteBuffers(1, &m_renderID));
	}

	void OpenGLIndexBuffer::bind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
	}

	void OpenGLIndexBuffer::unbind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
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
			MN_CORE_ASSERT(false, "shaderDataTypeToOpenGLType: Unknown ShaderDataType.");
			return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GLCALL(glCreateVertexArrays(1, &m_renderID));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GLCALL(glDeleteVertexArrays(1, &m_renderID));
	}

	void OpenGLVertexArray::bind() const
	{
		GLCALL(glBindVertexArray(m_renderID));
	}

	void OpenGLVertexArray::unbind() const
	{
		GLCALL(glBindVertexArray(0));
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		MN_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(),
			"OpenGLVertexArray::addVertexBuffer: Vertex buffer layout has no elements.");

		GLCALL(glBindVertexArray(m_renderID));
		vertexBuffer->bind();

		const auto& layout = vertexBuffer->getLayout();
		uint32_t stride = layout.getStride();
		for (const auto& element : layout)
		{
			GLCALL(glEnableVertexAttribArray(m_nAttributes));
			GLCALL(glVertexAttribPointer(m_nAttributes,
				shaderDataTypeComponentCount(element.type),
				shaderDataTypeToOpenGLType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				stride,
				reinterpret_cast<const void*>((uint64_t)(element.offset))));
			m_nAttributes++;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GLCALL(glBindVertexArray(m_renderID));
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;
	}

#pragma endregion

}