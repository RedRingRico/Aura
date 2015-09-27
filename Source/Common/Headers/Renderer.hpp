#ifndef __AURA_RENDERER_HPP__
#define __AURA_RENDERER_HPP__

#include <DataTypes.hpp>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace Aura
{
	class Window;

	class Renderer
	{
	public:
		Renderer( );
		~Renderer( );

		AUR_UINT32 Create( Aura::Window &p_Window );
		AUR_UINT32 Destroy( );

		void SetClearColour( AUR_FLOAT32 p_Red, AUR_FLOAT32 p_Green,
			AUR_FLOAT32 p_Blue );

		void SetClearFlags( AUR_BOOL p_Colour, AUR_BOOL p_Depth,
			AUR_BOOL p_Stencil );

		void Clear( );
		void SwapBuffers( );

	private:
		EGLDisplay	m_EGLDisplay;
		EGLSurface	m_EGLSurface;
		EGLContext	m_EGLContext;

		GLbitfield	m_ClearFlags;
	};
}

#endif // __AURA_RENDERER_HPP__

