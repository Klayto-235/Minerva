#pragma once

#include "Minerva/core/core.h"

#include <cstdint>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>


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

		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& vector) = 0;
		virtual void setFloat4(const std::string& name, const glm::vec4& vector) = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) = 0;

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