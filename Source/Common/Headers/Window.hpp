#ifndef __AURA_WINDOW_HPP__
#define __AURA_WINDOW_HPP__

#include <EGL/egl.h>
#include <DataTypes.hpp>

namespace Aura
{
	class Window
	{
	public:
		Window( );
		~Window( );

		AUR_UINT32 Create( );
		AUR_UINT32 Destroy( );
		AUR_UINT32 ProcessEvents( );

		EGLDisplay GetEGLDisplay( ) const;
		EGLSurface GetEGLSurface( ) const;

	private:
		EGLDisplay	m_EGLDisplay;
		EGLSurface	m_EGLSurface;
		EGLContext	m_EGLContext;
	};
}

#endif // __AURA_WINDOW_HPP__

