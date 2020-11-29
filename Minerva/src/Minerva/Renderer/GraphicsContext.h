#pragma once


namespace Minerva
{

	class GraphicsContext
	{
	public:
		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;
		GraphicsContext(const GraphicsContext & other) = delete;
		GraphicsContext& operator=(const GraphicsContext& other) = delete;

		virtual void swapBuffers() = 0;
	};

}