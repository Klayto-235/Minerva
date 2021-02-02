#pragma once

#include "Minerva/core/core.h"


namespace Minerva
{

	struct FramebufferProperties
	{
		uint32_t width = 0, height = 0;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;
		Framebuffer(const Framebuffer& other) = delete;
		Framebuffer& operator=(const Framebuffer& other) = delete;
		
		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getColorAttachmentRenderID() const = 0;

		virtual const FramebufferProperties& getProperties() const = 0;

		static Ref<Framebuffer> Framebuffer::create(const FramebufferProperties& properties);
	};

}