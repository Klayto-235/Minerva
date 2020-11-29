#pragma once
#include <string>


namespace Minerva
{

	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void bind() const;
		void unbind() const;
	private:
		unsigned int m_rendererID;
	};

}