#include "mnpch.h"
#include "Minerva/Renderer/Shader.h"
#include "Minerva/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Minerva
{

	Ref<Shader> Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLShader>(vertexSource, fragmentSource);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "Shader::create: RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Shader::create: Unknown RenderAPI::API.");
			return nullptr;
		}
	}

	Ref<Shader> Shader::create(const std::string& filePath)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return createRef<OpenGLShader>(filePath);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "Shader::create: RenderAPI::API::None is not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "Shader::create: Unknown RenderAPI::API.");
			return nullptr;
		}
	}

	void ShaderLibrary::add(Ref<Shader>& shader, const std::string& name)
	{
		MN_CORE_ASSERT(m_shaders.find(name) == m_shaders.end(), "ShaderLibrary::add: Shader already exists.");
		m_shaders.emplace(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filePath, const std::string& name)
	{
		auto shader = Shader::create(filePath);
		add(Shader::create(filePath), name);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filePath)
	{
		size_t nameBegin = filePath.find_last_of("/\\");
		if (nameBegin == std::string::npos) nameBegin = 0;
		else ++nameBegin;
		size_t nameEnd = filePath.rfind('.');
		if (nameEnd == std::string::npos || nameEnd <= nameBegin) nameEnd = filePath.length();
		MN_CORE_ASSERT(nameBegin != nameEnd, "ShaderLibrary::load: Invalid file path.");

		auto shader = Shader::create(filePath);
		add(shader, filePath.substr(nameBegin, nameEnd - nameBegin));
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name)
	{
		MN_CORE_ASSERT(m_shaders.find(name) != m_shaders.end(),
			"ShaderLibrary::get: Could not find shader \"{0}\".", name);
		return m_shaders.find(name)->second;
	}

}