#include <Game.hpp>
#include <GLES2/gl2.h>
#include <unistd.h>
#include <iostream>

namespace Aura
{
	Game::Game( )
	{
	}

	Game::~Game( )
	{
		m_Window.Destroy( );
	}

	AUR_UINT32 Game::Initialise( )
	{
		if( this->PlatformInitialise( ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Initialise] <ERROR> "
				"Failed to perform platform-specific initialisation" <<
				std::endl;

			return AUR_FAIL;
		}

		if( m_Window.Create( ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Initialise] <ERROR> "
				"Failed to create window" << std::endl;

			this->PlatformTerminate( );

			return AUR_FAIL;
		}

		if( m_Gamepad.Initialise( ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Initialise] <ERROR> "
				"Failed to initialise the gamepad" << std::endl;

			this->PlatformTerminate( );

			return AUR_FAIL;
		}

		glClearColor( 0.0f, 17.0f / 255.0f, 43.0f / 255.0f, 1.0f );

		return AUR_OK;
	}

	AUR_UINT32 Game::Execute( )
	{
		GAMEPAD_STATE GamepadState;
		AUR_BOOL Run = AUR_TRUE;

		while( Run )
		{
			m_Gamepad.GetState( &GamepadState );

			if( GamepadState.Buttons & GAMEPAD_BUTTON_START )
			{
				Run = AUR_FALSE;
			}

			glClear( GL_COLOR_BUFFER_BIT );
			eglSwapBuffers( m_Window.GetEGLDisplay( ),
				m_Window.GetEGLSurface( ) );
		}

		this->PlatformTerminate( );

		return AUR_OK;
	}
}

