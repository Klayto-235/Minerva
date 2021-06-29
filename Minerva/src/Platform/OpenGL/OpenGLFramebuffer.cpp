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

		glGenFramebuffers(1, &m_renderID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderID);

		glGenTextures(1, &m_colorAttachmentTextureRenderID);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachmentTextureRenderID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_properties.width, m_properties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachmentTextureRenderID, 0);

		glGenTextures(1, &m_depthAttachmentTextureRenderID);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachmentTextureRenderID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_properties.width, m_properties.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachmentTextureRenderID, 0);

		MN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > sc_maxSize || height > sc_maxSize)
		{
			MN_CORE_WARN("Attempted to rezize framebuffer to {0} x {1}.", width, height);
			return;
		}

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
