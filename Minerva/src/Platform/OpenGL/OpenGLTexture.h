#pragma once
#include "Minerva/core.h"
#include "Minerva/Renderer/Texture.h"


namespace Minerva
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		uint32_t getWidth() const override { return m_width; }
		uint32_t getHeight() const override { return m_height; }

		void bind(uint32_t slot = 0) const override;
	private:
#if defined MN_ENABLE_DEBUG_CODE
		std::string m_path;
#endif
		uint32_t m_width, m_height;
		uint32_t m_renderID;
	};

}