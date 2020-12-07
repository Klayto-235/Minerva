#include "mnpch.h"
#include "OpenGLTexture.h"
#include "OpenGL_core.h"

#include "stb_image.h"


namespace Minerva
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
#if defined MN_ENABLE_DEBUG_CODE
		: m_path(path)
#endif
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		MN_CORE_ASSERT(data, "OpenGLTexture2D::OpenGLTexture2D: Could not load image \"{0}\".", path);
		m_width = width;
		m_height = height;

		GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_renderID));

		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTextureParameteri(m_renderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCALL(glTextureStorage2D(m_renderID, 1, GL_RGB8, m_width, m_height));
		GLCALL(glTextureSubImage2D(m_renderID, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data));

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