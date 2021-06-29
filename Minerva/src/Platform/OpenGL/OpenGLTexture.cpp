#include "mnpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>

#include <stb_image.h>


namespace Minerva
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)
#if defined MN_ENABLE_DEBUG_CODE
		: m_filePath(filePath)
#endif
	{
		MN_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_uc* data = nullptr;
		stbi_set_flip_vertically_on_load(1);
		{
			MN_PROFILE_SCOPE("Load data - OpenGLTexture2D::OpenGLTexture2D");

			data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		}
		MN_CORE_ASSERT(data, "Could not load image \"{0}\".", filePath);
		m_width = width;
		m_height = height;
		if (channels == 4)
		{
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
		}
		else
		{
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
		}

		glGenTextures(1, &m_renderID);
		glBindTexture(GL_TEXTURE_2D, m_renderID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 1, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);
		{
			MN_PROFILE_SCOPE("Upload data - OpenGLTexture2D::OpenGLTexture2D");

			glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height,
				m_dataFormat, GL_UNSIGNED_BYTE, data);
		}

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_width(width), m_height(height), m_internalFormat(GL_RGBA8), m_dataFormat(GL_RGBA)
	{
		MN_PROFILE_FUNCTION();

		glGenTextures(1, &m_renderID);
		glBindTexture(GL_TEXTURE_2D, m_renderID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 1, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		MN_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_renderID);
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(size == m_width * m_height * (m_dataFormat == GL_RGBA ? 4 : 3),
			"Parameter size must be equal to texture size.");
		glBindTexture(GL_TEXTURE_2D, m_renderID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		MN_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_renderID);
	}

}