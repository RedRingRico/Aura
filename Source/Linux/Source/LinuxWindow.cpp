#include <Window.hpp>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <iostream>

namespace Aura
{
	AUR_UINT32 Window::Create( )
	{
		m_pDisplay = XOpenDisplay( AUR_NULL );

		if( m_pDisplay == AUR_NULL )
		{
			std::cout << "[Aura::Window::Create] <ERROR> "
				"Failed to create display" << std::endl;

			return AUR_FAIL;
		}

		::Window RootWindow = DefaultRootWindow( m_pDisplay );
		XSetWindowAttributes WindowAttributes;

		WindowAttributes.event_mask = ExposureMask | PointerMotionMask |
			KeyPressMask | KeyReleaseMask;

		m_Window = XCreateWindow( m_pDisplay, RootWindow,
			0, 1000, 800, 480, 0,
			CopyFromParent, InputOutput, CopyFromParent, CWEventMask,
			&WindowAttributes );
		XMapWindow( m_pDisplay, m_Window );
		XStoreName( m_pDisplay, m_Window, "Aura" );

		m_EGLDisplay = eglGetDisplay( ( EGLNativeDisplayType )( m_pDisplay ) );

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
			m_Window, AUR_NULL );

		if( m_EGLSurface == EGL_NO_SURFACE )
		{
			std::cout << "[Aura::Window::Create] <ERROR> "
				"Failed when creating the window surface" << std::endl;

			return AUR_FAIL;
		}

		return AUR_OK;
	}
}

