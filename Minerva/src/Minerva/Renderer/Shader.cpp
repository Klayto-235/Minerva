#include "mnpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Minerva
{

	Ref<Shader> Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		case RenderAPI::API::None:
			MN_CORE_ASSERT(false, "OpenGLShader::create: RendererAPI::None is currently not supported.");
			return nullptr;
		default:
			MN_CORE_ASSERT(false, "OpenGLShader::create: Unknown RendererAPI.");
			return nullptr;
		}
	}

}