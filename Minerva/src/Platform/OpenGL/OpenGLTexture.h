#pragma once

#include "Minerva/Renderer/Texture.h"

#include <glad/glad.h>


namespace Minerva
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filePath);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		~OpenGLTexture2D();

		uint32_t getWidth() const override { return m_width; }
		uint32_t getHeight() const override { return m_height; }

		void setData(void* data, uint32_t size) override;

		void bind(uint32_t slot = 0) const override;

		bool operator==(const Texture& other) const override 
		{
			return static_cast<const OpenGLTexture2D&>(other).m_renderID == m_renderID;
		}
	private:
#if defined MN_ENABLE_DEBUG_CODE
		std::string m_filePath;
#endif
		uint32_t m_width, m_height;
		uint32_t m_renderID;
		GLenum m_internalFormat, m_dataFormat;
	};

}