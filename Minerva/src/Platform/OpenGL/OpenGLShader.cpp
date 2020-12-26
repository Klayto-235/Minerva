#include "mnpch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGL_core.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <cctype>
#include <string_view>
#include <array>


namespace Minerva
{

	static constexpr GLenum stringToShaderType(const std::string_view& strView)
	{
		using namespace std::literals::string_view_literals;
		if (strView == "vertex"sv)									return GL_VERTEX_SHADER;
		else if (strView == "fragment"sv || strView == "pixel"sv)	return GL_FRAGMENT_SHADER;
		else
		{
			MN_CORE_ASSERT(false, "stringToShaderType: Unknown shader type.");
			return 0;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = readFile(filePath);
		auto shaderSources = parse(source);
		compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		GLCALL(glDeleteProgram(m_renderID));
	}

	void OpenGLShader::bind() const
	{
		GLCALL(glUseProgram(m_renderID));
	}

	void OpenGLShader::unbind() const
	{
		GLCALL(glUseProgram(0));
	}

	void OpenGLShader::setInt(const std::string& name, int value)
	{
		uploadUniformInt(name, value);
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& vector)
	{
		uploadUniformFloat3(name, vector);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& vector)
	{
		uploadUniformFloat4(name, vector);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& matrix)
	{
		uploadUniformMat4(name, matrix);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value)
	{
		GLCALL(GLint location = glGetUniformLocation(m_renderID, name.c_str()));
		GLCALL(glUniform1i(location, value));
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, float value)
	{
		GLCALL(GLint location = glGetUniformLocation(m_renderID, name.c_str()));
		GLCALL(glUniform1f(location, value));
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& vector)
	{
		GLCALL(GLint location = glGetUniformLocation(m_renderID, name.c_str()));
		GLCALL(glUniform2f(location, vector.x, vector.y));
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& vector)
	{
		GLCALL(GLint location = glGetUniformLocation(m_renderID, name.c_str()));
		GLCALL(glUniform3f(location, vector.x, vector.y, vector.z));
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& vector)
	{
		GLCALL(GLint location = glGetUniformLocation(m_renderID, name.c_str()));
		GLCALL(glUniform4f(location, vector.x, vector.y, vector.z, vector.w));
	}

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLCALL(GLint location = glGetUniformLocation(m_renderID, name.c_str()));
		GLCALL(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLCALL(GLint location = glGetUniformLocation(m_renderID, name.c_str()));
		GLCALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	std::string OpenGLShader::readFile(const std::string& filePath)
	{
		std::string data;
		std::ifstream ifs(filePath, std::ios::in | std::ios::binary);
		if (ifs)
		{
			ifs.seekg(0, std::ios::end);
			auto size = ifs.tellg();
			if (size != -1)
			{
				data.resize(size);
				ifs.seekg(0, std::ios::beg);
				ifs.read(&data[0], data.size());
				ifs.close();
			}
			else
			{
				MN_CORE_ERROR("Could not read from file \"{0}\".", filePath);
			}
		}
		else
		{
			MN_CORE_ERROR("Could not open file \"{0}\".", filePath);
		}
		return data;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::parse(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		constexpr char typeToken[] = "#type";
		constexpr size_t tokenLength = sizeof(typeToken);
		size_t tokenBegin = source.find(typeToken, 0);
		bool syntaxError = false;
		while (tokenBegin != std::string::npos)
		{
			size_t lineBegin = source.rfind('\n', tokenBegin);
			if (lineBegin == std::string::npos) lineBegin = 0;
			else ++lineBegin;
			if (source.find_first_not_of(" \f\t\v\r", lineBegin) != tokenBegin)
			{
				syntaxError = true;
				break;
			}

			const size_t typeStringBegin = source.find_first_not_of(" \f\t\v\r", tokenBegin + tokenLength);
			const size_t typeStringEnd = source.find_first_of(" \f\t\v\r\n", typeStringBegin);

			const size_t lineEnd = source.find('\n', tokenBegin + tokenLength);
			if (lineEnd == std::string::npos ||
				source.find_first_not_of(" \f\t\v\r", typeStringEnd) != lineEnd)
			{
				syntaxError = true;
				break;
			}

			const GLenum shaderType = stringToShaderType(
				std::string_view(source.data() + typeStringBegin, typeStringEnd - typeStringBegin));
			if (shaderSources.find(shaderType) != shaderSources.end())
			{ // Multiple shader objects of the same type can be linked, but we don't support that atm.
				syntaxError = true;
				break;
			}
			const size_t shaderSourceBegin = lineEnd + 1;
			tokenBegin = source.find(typeToken, shaderSourceBegin);
			const size_t shaderSourceLength = (tokenBegin == std::string::npos ?
				source.length() : source.rfind('\n', tokenBegin)) - shaderSourceBegin;
			shaderSources.emplace(shaderType,
				std::string_view(source.data() + shaderSourceBegin, shaderSourceLength));
		}
		MN_CORE_ASSERT(!syntaxError, "OpenGLShader::parse: Shader source syntax error.");
		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		MN_CORE_ASSERT(shaderSources.size() <= 2,
			"OpenGLShader::compile: Currently, only two shaders are supported.");
		std::array<GLuint, 2> shaderIDs = { 0 };

		m_renderID = GLCALL(glCreateProgram());

		int shaderIDidx = 0;
		bool compilationError = false;
		for (auto& kv : shaderSources)
		{
			GLuint shaderID = GLCALL(glCreateShader(kv.first));
			shaderIDs[shaderIDidx++] = shaderID;

			const GLchar* shaderSource = kv.second.c_str();
			GLCALL(glShaderSource(shaderID, 1, &shaderSource, nullptr));
			GLCALL(glCompileShader(shaderID));

			GLint result = 0;
			GLCALL(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result));
			if (result == GL_FALSE)
			{
				GLint maxLength = 0;
				GLCALL(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength));

				std::vector<GLchar> infoLog(maxLength);
				GLCALL(glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]));

				MN_CORE_ERROR("{0}", infoLog.data());
				MN_CORE_ASSERT(false, "OpenGLShader::compile: Could not compile shader.");
				
				compilationError = true;
				break;
			}
		}

		if (!compilationError)
		{
			for (auto id : shaderIDs) GLCALL(glAttachShader(m_renderID, id));
			GLCALL(glLinkProgram(m_renderID));

			GLint result = 0;
			GLCALL(glGetProgramiv(m_renderID, GL_LINK_STATUS, &result));
			if (result == GL_FALSE)
			{
				GLint maxLength = 0;
				GLCALL(glGetProgramiv(m_renderID, GL_INFO_LOG_LENGTH, &maxLength));

				std::vector<GLchar> infoLog(maxLength);
				GLCALL(glGetProgramInfoLog(m_renderID, maxLength, &maxLength, &infoLog[0]));

				GLCALL(glDeleteProgram(m_renderID));

				MN_CORE_ERROR("{0}", infoLog.data());
				MN_CORE_ASSERT(false, "OpenGLShader::compile: Could not link shader program.");
			}
			else for (auto id : shaderIDs) GLCALL(glDetachShader(m_renderID, id));
		}

		while (shaderIDidx--) GLCALL(glDeleteShader(shaderIDs[shaderIDidx]));
	}

}