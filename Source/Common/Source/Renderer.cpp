#include <Renderer.hpp>
#include <Window.hpp>
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

