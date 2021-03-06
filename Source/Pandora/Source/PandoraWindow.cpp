#include <Window.hpp>
#include <iostream>

namespace Aura
{
	AUR_UINT32 Window::Create( )
	{
		m_EGLDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );

		if( m_EGLDisplay == EGL_NO_DISPLAY )
		{
			std::cout << "[Aura::Window::Create] <ERROR> "
				"No display available" << std::endl;

			return AUR_FAIL;
		}

		EGLint EGLMajor, EGLMinor;

		if( eglInitialize( m_EGLDisplay, &EGLMajor, &EGLMinor ) != EGL_TRUE )
		{
			std::cout << "[Aura::Window::Create] <ERROR> "
				"Failed to initialise EGL" << std::endl;

			return AUR_FAIL;
		}

		if( ( EGLMajor < 1 ) || ( ( EGLMajor == 1 ) && ( EGLMinor < 4 ) ) )
		{
			std::cout << "[Aura::Window::Create] <ERROR> "
				"EGL version is not 1.4 or greater.  Found version: " <<
				EGLMajor << "." << EGLMinor << std::endl;

			return AUR_FAIL;
		}

		EGLint ConfigCount;

		EGLint EGLAttributes[ ] =
		{
			EGL_RED_SIZE,			5,
			EGL_GREEN_SIZE,			6,
			EGL_BLUE_SIZE,			5,
			EGL_ALPHA_SIZE,			0,
			EGL_DEPTH_SIZE,			24,
			EGL_STENCIL_SIZE,		8,
			EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};

		if( eglChooseConfig( m_EGLDisplay, EGLAttributes, &m_EGLConfig, 1,
			&ConfigCount ) != EGL_TRUE )
		{
			std::cout << "[Aura::Window::Create] <ERROR> "
				"Unable to choose an EGL configuration" << std::endl;

			return AUR_FAIL;
		}

		m_EGLSurface = eglCreateWindowSurface( m_EGLDisplay, m_EGLConfig,
			AUR_NULL, AUR_NULL );

		if( m_EGLSurface == EGL_NO_SURFACE )
		{
			std::cout << "[Aura::Window::Create] <ERROR> "
				"Failed when creating the window surface" << std::endl;

			return AUR_FAIL;
		}

		return AUR_OK;
	}
}

