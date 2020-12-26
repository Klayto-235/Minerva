#include "mnpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGL_core.h"

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
		MN_CORE_ASSERT(data, "OpenGLTexture2D::OpenGLTexture2D: Could not load image \"{0}\".", filePath);
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

		GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_renderID));

		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCALL(glTextureStorage2D(m_renderID, 1, m_internalFormat, m_width, m_height));
		{
			MN_PROFILE_SCOPE("Upload data - OpenGLTexture2D::OpenGLTexture2D");

			GLCALL(glTextureSubImage2D(m_renderID, 0, 0, 0, m_width, m_height,
				m_dataFormat, GL_UNSIGNED_BYTE, data));
		}

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_width(width), m_height(height), m_internalFormat(GL_RGBA8), m_dataFormat(GL_RGBA)
	{
		MN_PROFILE_FUNCTION();

		GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_renderID));

		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCALL(glTextureStorage2D(m_renderID, 1, m_internalFormat, m_width, m_height));
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		MN_PROFILE_FUNCTION();

		GLCALL(glDeleteTextures(1, &m_renderID));
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(size == m_width * m_height * (m_dataFormat == GL_RGBA ? 4 : 3),
			"OpenGLTexture2D::setData: Parameter size must be equal to texture size.");
		glTextureSubImage2D(m_renderID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		MN_PROFILE_FUNCTION();

		GLCALL(glBindTextureUnit(slot, m_renderID));
	}

}