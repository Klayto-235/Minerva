#pragma once

#include "Minerva/Renderer/Shader.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

// TEMPORARAY
typedef unsigned int GLenum;


namespace Minerva
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader();

		void bind() const override;
		void unbind() const override;

		void setInt(const std::string& name, int value) override;
		void setFloat3(const std::string& name, const glm::vec3& vector) override;
		void setFloat4(const std::string& name, const glm::vec4& vector) override;
		void setMat4(const std::string& name, const glm::mat4& matrix) override;

		void uploadUniformInt(const std::string& name, int value);

		void uploadUniformFloat(const std::string& name, float value);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& vector);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& vector);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& vector);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string readFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> parse(const std::string& source);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		uint32_t m_renderID;
	};

}