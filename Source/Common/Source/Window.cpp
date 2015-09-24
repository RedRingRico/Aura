#include <Window.hpp>
#include <iostream>

namespace Aura
{
	Window::Window( ) :
		m_EGLDisplay( EGL_NO_DISPLAY ),
		m_EGLSurface( EGL_NO_SURFACE ),
		m_EGLContext( EGL_NO_CONTEXT ),
		m_pDisplay( AUR_NULL ),
		m_Window( 0 )
	{
	}

	Window::~Window( )
	{
		this->Destroy( );
	}


	AUR_UINT32 Window::Destroy( )
	{
		if( m_EGLContext != EGL_NO_CONTEXT )
		{
			eglDestroyContext( m_EGLDisplay, m_EGLContext );
			m_EGLContext = EGL_NO_CONTEXT;
		}

		if( m_EGLSurface != EGL_NO_SURFACE )
		{
			eglDestroySurface( m_EGLDisplay, m_EGLSurface );
			m_EGLSurface = EGL_NO_SURFACE;
		}

		if( m_EGLDisplay != EGL_NO_DISPLAY )
		{
			eglMakeCurrent( m_EGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
				EGL_NO_CONTEXT );
			eglTerminate( m_EGLDisplay );
			m_EGLDisplay = EGL_NO_DISPLAY;
		}

		if( m_Window )
		{
			XDestroyWindow( m_pDisplay, m_Window );
			m_Window = 0;
		}

		if( m_pDisplay )
		{
			XCloseDisplay( m_pDisplay );
			m_pDisplay = AUR_NULL;
		}

		return AUR_OK;
	}

	AUR_UINT32 Window::ProcessEvents( )
	{
		return AUR_OK;
	}

	EGLDisplay Window::GetEGLDisplay( ) const
	{
		return m_EGLDisplay;
	}

	EGLSurface Window::GetEGLSurface( ) const
	{
		return m_EGLSurface;
	}
}

