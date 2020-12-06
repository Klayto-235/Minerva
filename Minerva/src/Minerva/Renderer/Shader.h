#pragma once

#include <cstdint>
#include <string>


namespace Minerva
{

	class Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static Shader* create(const std::string& vertexSource, const std::string& fragmentSource);
	};

}