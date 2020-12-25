#pragma once

#include "Minerva/core/core.h"

#include <glad/glad.h>


#if defined MN_ENABLE_OPENGL_ERRORS || MN_ENABLE_OPENGL_ASSERTS
	inline void checkGLErrors(const char* call, const char* file, int line)
	{
		GLenum error;
		unsigned char nErrors = 0;
		while ((error = glGetError()) != GL_NO_ERROR && nErrors < 64)
		{
			MN_CORE_ERROR("OpenGL error ({0}): {1}; {2}:{3}.", error, call, file, line);
			++nErrors;
		}

		if (nErrors >= 64)
			MN_CORE_ERROR("Infinite OpenGL error loop detected; broke out after {0} iterations.", nErrors);
	#if defined MN_ENABLE_OPENGL_ASSERTS
		MN_CORE_ASSERT(nErrors == 0, "OpenGL error.");
	#endif
	}

	#define GLCALL(x) x; checkGLErrors(#x, __FILE__, __LINE__)
#else
	#define GLCALL(x) x
#endif