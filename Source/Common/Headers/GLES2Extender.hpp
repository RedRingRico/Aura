#ifndef __AURA_GLES2EXTENDER_HPP__
#define __AURA_GLES2EXTENDER_HPP__

#include <DataTypes.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

extern "C"
{
	extern PFNGLGENVERTEXARRAYSOESPROC		__glGenVertexArrays;
	extern PFNGLDELETEVERTEXARRAYSOESPROC	__glDeleteVertexArrays;
	extern PFNGLBINDVERTEXARRAYOESPROC		__glBindVertexArray;
}

#define glGenVertexArrays		__glGenVertexArrays
#define glDeleteVertexArrays	__glDeleteVertexArrays
#define glBindVertexArray		__glBindVertexArray

namespace Aura
{
	AUR_UINT32 InitialiseGLES2Extensions( );

	void PrintGLES2Extensions( );
}

#endif // __AURA_GLES2EXTENDER_HPP__

