#pragma once

#include "Minerva/core/core.h"

#include <cstdint>
#include <string>
#include <unordered_map>


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

		static Ref<Shader> create(const std::string& vertexSource, const std::string& fragmentSource);
		static Ref<Shader> create(const std::string& filePath);
	};

	class ShaderLibrary
	{
	public:
		void add(Ref<Shader>& shader, const std::string& name);
		Ref<Shader> load(const std::string& filePath, const std::string& name);
		Ref<Shader> load(const std::string& filePath);

		Ref<Shader> get(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};

}