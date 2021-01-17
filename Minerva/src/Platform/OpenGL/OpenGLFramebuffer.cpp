#include "mnpch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>


namespace Minerva
{

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferProperties& properties)
		: m_properties(properties)
	{
		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_renderID);
		glDeleteTextures(1, &m_colorAttachmentTextureRenderID);
		glDeleteTextures(1, &m_depthAttachmentTextureRenderID);
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (m_renderID != 0)
		{
			glDeleteFramebuffers(1, &m_renderID);
			glDeleteTextures(1, &m_colorAttachmentTextureRenderID);
			glDeleteTextures(1, &m_depthAttachmentTextureRenderID);
		}

		glCreateFramebuffers(1, &m_renderID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachmentTextureRenderID);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachmentTextureRenderID);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_properties.width, m_properties.height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachmentTextureRenderID, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachmentTextureRenderID);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachmentTextureRenderID);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_properties.width, m_properties.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachmentTextureRenderID, 0);

		MN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"OpenGLFramebuffer::invalidate: Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		m_properties.width = width;
		m_properties.height = height;

		invalidate();
	}

	void OpenGLFramebuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);
		glViewport(0, 0, m_properties.width, m_properties.height);
	}

	void OpenGLFramebuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
