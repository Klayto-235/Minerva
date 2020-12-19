#pragma once

#include "Minerva/core/core.h"

#include <cstdint>
#include <string>


namespace Minerva
{

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> create(const std::string& filePath);
	};

}