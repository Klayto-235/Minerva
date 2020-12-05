#include "mnpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"


namespace Minerva
{

	RenderAPI* RenderCommand::s_renderAPI = new OpenGLRenderAPI;

}