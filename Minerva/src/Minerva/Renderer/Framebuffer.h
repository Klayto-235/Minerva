#pragma once

#include "Minerva/core/core.h"


namespace Minerva
{

	struct FramebufferProperties
	{
		uint32_t width, height;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;
		Framebuffer(const Framebuffer& other) = delete;
		Framebuffer& operator=(const Framebuffer& other) = delete;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getColorAttachmentRenderID() const = 0;

		virtual const FramebufferProperties& getProperties() const = 0;

		static Ref<Framebuffer> Framebuffer::create(const FramebufferProperties& properties);
	};

}