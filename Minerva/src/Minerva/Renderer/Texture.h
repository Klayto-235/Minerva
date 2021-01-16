#pragma once

#include "Minerva/core/core.h"

#include <cstdint>
#include <string>


namespace Minerva
{

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;
		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;

		virtual uint32_t getRenderID() const = 0; // needed for ImGui

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> create(const std::string& filePath);
		static Ref<Texture2D> create(uint32_t width, uint32_t height);
	};

}