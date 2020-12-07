#include "mnpch.h"
#include "OpenGLShader.h"
#include "OpenGL_core.h"

#include <glm/gtc/type_ptr.hpp>


namespace Minerva
{

	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = GLCALL(glCreateShader(GL_VERTEX_SHADER));

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSource.c_str();
		GLCALL(glShaderSource(vertexShader, 1, &source, nullptr));

		// Compile the vertex shader
		GLCALL(glCompileShader(vertexShader));

		GLint isCompiled = 0;
		GLCALL(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCALL(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCALL(glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]));

			// We don't need the shader anymore.
			GLCALL(glDeleteShader(vertexShader));

			MN_CORE_ERROR("{0}", infoLog.data());
			MN_CORE_ASSERT(false, "OpenGLShader::OpenGLShader: Could not compile vertex shader.");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = GLCALL(glCreateShader(GL_FRAGMENT_SHADER));

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSource.c_str();
		GLCALL(glShaderSource(fragmentShader, 1, &source, nullptr));

		// Compile the fragment shader
		GLCALL(glCompileShader(fragmentShader));

		GLCALL(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCALL(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCALL(glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]));

			// We don't need the shader anymore.
			GLCALL(glDeleteShader(fragmentShader));
			// Either of them. Don't leak shaders.
			GLCALL(glDeleteShader(vertexShader));

			MN_CORE_ERROR("{0}", infoLog.data());
			MN_CORE_ASSERT(false, "OpenGLShader::OpenGLShader: Could not compile fragment shader.");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_renderID = GLCALL(glCreateProgram());

		// Attach our shaders to our program
		GLCALL(glAttachShader(m_renderID, vertexShader));
		GLCALL(glAttachShader(m_renderID, fragmentShader));

		// Link our program
		GLCALL(glLinkProgram(m_renderID));

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		GLCALL(glGetProgramiv(m_renderID, GL_LINK_STATUS, (int*)&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCALL(glGetProgramiv(m_renderID, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCALL(glGetProgramInfoLog(m_renderID, maxLength, &maxLength, &infoLog[0]));

			// We don't need the program anymore.
			GLCALL(glDeleteProgram(m_renderID));
			// Don't leak shaders either.
			GLCALL(glDeleteShader(vertexShader));
			GLCALL(glDeleteShader(fragmentShader));

			MN_CORE_ERROR("{0}", infoLog.data());
			MN_CORE_ASSERT(false, "OpenGLShader::OpenGLShader: Could not link shader program.");
			return;
		}

		// Always detach shaders after a successful link.
		GLCALL(glDetachShader(m_renderID, vertexShader));
		GLCALL(glDetachShader(m_renderID, fragmentShader));
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

}