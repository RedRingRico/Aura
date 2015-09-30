#include <GLES2Extender.hpp>
#include <EGL/egl.h>
#include <iostream>
#include <string>
#include <vector>

PFNGLGENVERTEXARRAYSOESPROC		__glGenVertexArrays = AUR_NULL;
PFNGLDELETEVERTEXARRAYSOESPROC	__glDeleteVertexArrays = AUR_NULL;
PFNGLBINDVERTEXARRAYOESPROC		__glBindVertexArray = AUR_NULL;

namespace Aura
{
	AUR_UINT32 InitialiseGLES2Extensions( )
	{
		AUR_BOOL Initialised = AUR_FALSE;

		std::cout << "[Aura::InitialiseGLESExtensions] <INFO> "
			"Binding glGenVertexArraysOES ...    ";

		Initialised |= ( ( __glGenVertexArrays =
			( PFNGLGENVERTEXARRAYSOESPROC )eglGetProcAddress(
				"glGenVertexArraysOES" ) ) == AUR_NULL );

		if( __glGenVertexArrays )
		{
			std::cout << "[ OK ]" << std::endl;
		}
		else
		{
			std::cout << "[FAIL]" << std::endl;
		}

		std::cout << "[Aura::InitialiseGLESExtensions] <INFO> "
			"Binding glDeleteVertexArraysOES ... ";

		Initialised |= ( ( __glDeleteVertexArrays =
			( PFNGLDELETEVERTEXARRAYSOESPROC )eglGetProcAddress(
				"glDeleteVertexArraysOES" ) ) == AUR_NULL );

		if( __glDeleteVertexArrays )
		{
			std::cout << "[ OK ]" << std::endl;
		}
		else
		{
			std::cout << "[FAIL]" << std::endl;
		}

		std::cout << "[Aura::InitialiseGLESExtensions] <INFO> "
			"Binding glBindVertexArrayOES ...    ";

		Initialised |= ( ( __glBindVertexArray =
			( PFNGLBINDVERTEXARRAYOESPROC )eglGetProcAddress(
				"glBindVertexArrayOES" ) ) == AUR_NULL );

		if( __glBindVertexArray )
		{
			std::cout << "[ OK ]" << std::endl;
		}
		else
		{
			std::cout << "[FAIL]" << std::endl;
		}

		return Initialised ? AUR_FAIL : AUR_OK;
	}

	void PrintGLES2Extensions( )
	{
		std::string Extensions(
			reinterpret_cast< const char * >( glGetString( GL_EXTENSIONS ) ) );
		std::vector< std::string > ExtensionArray;
		size_t ExtensionStart = 0, ExtensionEnd = 0;

		while( ExtensionEnd < Extensions.size( ) )
		{
			if( Extensions[ ExtensionEnd ] == ' ' )
			{
				std::string Extension = Extensions.substr( ExtensionStart,
					ExtensionEnd - ExtensionStart );
				ExtensionArray.push_back( Extension );
				// Skip the space
				ExtensionStart = ExtensionEnd + 1;
			}
			else if( ExtensionEnd == Extensions.size( ) - 1 )
			{
				std::string Extension = Extensions.substr( ExtensionStart,
					Extensions.size( ) - ExtensionStart );
				ExtensionArray.push_back( Extension );
			}

			++ExtensionEnd;
		}

		std::cout << "[Aura::PrintGLES2Extensions] <INFO> "
			<< ExtensionArray.size( ) << " extensions available" << std::endl;

		for( size_t Extension = 0; Extension < ExtensionArray.size( );
			++Extension )
		{
			std::cout << "\t" << ExtensionArray[ Extension ] << std::endl;
		}
	}
}

