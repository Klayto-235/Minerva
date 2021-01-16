#pragma once

#include "Minerva/Renderer/Framebuffer.h"


namespace Minerva
{

	class OpenGLFramebuffer: public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferProperties& properties);
		~OpenGLFramebuffer();

		void invalidate();

		void bind() const override;
		void unbind() const override;

		uint32_t getColorAttachmentRenderID() const override { return m_colorAttachmentTextureRenderID; }

		const FramebufferProperties& getProperties() const override { return m_properties; }
	private:
		uint32_t m_renderID;
		uint32_t m_colorAttachmentTextureRenderID, m_depthAttachmentTextureRenderID;
		FramebufferProperties m_properties;
	};

}