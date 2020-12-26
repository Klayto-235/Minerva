#include "mnpch.h"
#include "Minerva/Renderer/RenderCommand.h"


namespace Minerva
{

	Scope<RenderAPI> RenderCommand::s_renderAPI = RenderAPI::create();

}