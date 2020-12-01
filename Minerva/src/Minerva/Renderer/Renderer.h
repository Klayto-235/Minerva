#pragma once

namespace Minerva
{

	enum class RendererAPI
	{
		None = 0, OpenGL
	};

	class Renderer
	{
	public:
		static RendererAPI GetAPI() { return s_rendererAPI; }
	private:
		static RendererAPI s_rendererAPI;
	};

}