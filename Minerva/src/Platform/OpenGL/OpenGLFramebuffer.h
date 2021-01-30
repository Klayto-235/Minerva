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
		void resize(uint32_t width, uint32_t height) override;

		void bind() const override;
		void unbind() const override;

		uint32_t getColorAttachmentRenderID() const override { return m_colorAttachmentTextureRenderID; }

		const FramebufferProperties& getProperties() const override { return m_properties; }
	private:
		uint32_t m_renderID = 0;
		uint32_t m_colorAttachmentTextureRenderID = 0, m_depthAttachmentTextureRenderID = 0;
		FramebufferProperties m_properties;

		static constexpr uint32_t sc_maxSize = 8192;
	};

}