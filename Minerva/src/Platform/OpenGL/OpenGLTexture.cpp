#include "mnpch.h"
#include "OpenGLTexture.h"
#include "OpenGL_core.h"

#include "stb_image.h"


namespace Minerva
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)
#if defined MN_ENABLE_DEBUG_CODE
		: m_filePath(filePath)
#endif
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		MN_CORE_ASSERT(data, "OpenGLTexture2D::OpenGLTexture2D: Could not load image \"{0}\".", filePath);
		m_width = width;
		m_height = height;
		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_renderID));

		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCALL(glTextureStorage2D(m_renderID, 1, internalFormat, m_width, m_height));
		GLCALL(glTextureSubImage2D(m_renderID, 0, 0, 0, m_width,
			m_height, dataFormat, GL_UNSIGNED_BYTE, data));

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GLCALL(glDeleteTextures(1, &m_renderID));
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		GLCALL(glBindTextureUnit(slot, m_renderID));
	}

}