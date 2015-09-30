#include <Renderer.hpp>
#include <Window.hpp>
#include <GLES2Extender.hpp>
#include <iostream>

namespace Aura
{
	Renderer::Renderer( ) :
		m_EGLDisplay( EGL_NO_DISPLAY ),
		m_EGLSurface( EGL_NO_SURFACE ),
		m_EGLContext( EGL_NO_CONTEXT ),
		m_ClearFlags( 0 )
	{
	}

	Renderer::~Renderer( )
	{
		this->Destroy( );
	}

	AUR_UINT32 Renderer::Create( Aura::Window &p_Window )
	{
		if( p_Window.GetEGLDisplay( ) == EGL_NO_DISPLAY )
		{
			return AUR_FAIL;
		}

		if( p_Window.GetEGLSurface( ) == EGL_NO_SURFACE )
		{
			return AUR_FAIL;
		}

		m_EGLDisplay = p_Window.GetEGLDisplay( );
		m_EGLSurface = p_Window.GetEGLSurface( );

		EGLint ContextAttribs[ ] =
		{
			EGL_CONTEXT_CLIENT_VERSION,	2,
			EGL_NONE
		};

		m_EGLContext = eglCreateContext( m_EGLDisplay,
			p_Window.GetEGLConfig( ), EGL_NO_CONTEXT, ContextAttribs );

		if( m_EGLContext == EGL_NO_CONTEXT )
		{
			std::cout << "[Aura::Renderer::Create] <ERROR> "
				"Failed to create an EGL context" << std::endl;

			return AUR_FAIL;
		}

		if( eglMakeCurrent( m_EGLDisplay, m_EGLSurface, m_EGLSurface,
			m_EGLContext ) != EGL_TRUE )
		{
			std::cout << "[Aura::Renderer::Create] <ERROR> "
				"Unable to make the EGL context current" << std::endl;

			return AUR_FAIL;
		}

#if defined AURA_BUILD_DEBUG
		PrintGLES2Extensions( );
#endif // AURA_BUILD_DEBUG

		if( InitialiseGLES2Extensions( ) != AUR_OK )
		{
			std::cout << "[Aura::Renderer::Create] <ERROR> "
				"Failed to initialise OpenGL | ES 2.0 extensions" << std::endl;

			return AUR_FAIL;
		}

#if defined AURA_BUILD_DEBUG
		std::cout << "[Aura::Renderer::Create] <INFO> GLES Vendor: " <<
			glGetString( GL_VENDOR ) << std::endl;
		std::cout << "[Aura::Renderer::Create] <INFO> GLES Renderer: " <<
			glGetString( GL_RENDERER ) << std::endl;
		std::cout << "[Aura::Renderer::Create] <INFO> GLES Version: " <<
			glGetString( GL_VERSION ) << std::endl;
		std::cout << "[Aura::Renderer::Create] <INFO> GLES GLSL "
			"Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) <<
			std::endl;

		std::cout << "[Aura::Renderer::Create] <INFO> Capabilities:" <<
			std::endl;

		GLint MaxVertexAttribs;
		GLint MaxUniformVectors;
		GLint MaxVaryingVectors;
		GLint MaxVertexTextureImageUnits;
		GLint MaxCombinedTextureImageUnits;
		GLint MaxTextureImageUnits;
		GLint MaxFragmentUniformVectors;

		glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &MaxVertexAttribs );
		glGetIntegerv( GL_MAX_VERTEX_UNIFORM_VECTORS, &MaxUniformVectors );
		glGetIntegerv( GL_MAX_VARYING_VECTORS, &MaxVaryingVectors );
		glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
			&MaxVertexTextureImageUnits );
		glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			&MaxCombinedTextureImageUnits );
		glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureImageUnits );
		glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_VECTORS,
			&MaxFragmentUniformVectors );

		std::cout << "\tMaximum vertex shader attributes:      " <<
			MaxVertexAttribs << std::endl;
		std::cout << "\tMaximum vertex shader uniform vectors: " <<
			MaxUniformVectors << std::endl;
		std::cout << "\tMaximum varying vectors:               " <<
			MaxVaryingVectors << std::endl;
		std::cout << "\tMaximum vertex texture image units:    " <<
			MaxVertexTextureImageUnits << std::endl;
		std::cout << "\tMaximum combined texture image units:  " <<
			MaxCombinedTextureImageUnits << std::endl;
		std::cout << "\tMaximum texture image units:           " <<
			MaxTextureImageUnits <<std::endl;
		std::cout << "\tMaximum fragment uniform vectors:      " <<
			MaxFragmentUniformVectors << std::endl;
#endif // AURA_BUILD_DEBUG

		return AUR_OK;
	}

	AUR_UINT32 Renderer::Destroy( )
	{
		if( m_EGLContext != EGL_NO_CONTEXT )
		{
			eglDestroyContext( m_EGLDisplay, m_EGLContext );
			m_EGLContext = EGL_NO_CONTEXT;
		}

		return AUR_OK;
	}

	void Renderer::SetClearColour( AUR_FLOAT32 p_Red, AUR_FLOAT32 p_Green,
		AUR_FLOAT32 p_Blue )
	{
		glClearColor( p_Red, p_Green, p_Blue, 1.0f );
	}

	void Renderer::SetClearFlags( AUR_BOOL p_Colour, AUR_BOOL p_Depth,
		AUR_BOOL p_Stencil )
	{
		m_ClearFlags = 0;

		if( p_Colour == AUR_TRUE )
		{
			m_ClearFlags |= GL_COLOR_BUFFER_BIT;
		}
		
		if( p_Depth == AUR_TRUE )
		{
			m_ClearFlags |= GL_DEPTH_BUFFER_BIT;
		}

		if( p_Stencil == AUR_TRUE )
		{
			m_ClearFlags |= GL_STENCIL_BUFFER_BIT;
		}
	}

	void Renderer::Clear( )
	{
		glClear( m_ClearFlags );
	}

	void Renderer::SwapBuffers( )
	{
		eglSwapBuffers( m_EGLDisplay, m_EGLSurface );
	}
}

