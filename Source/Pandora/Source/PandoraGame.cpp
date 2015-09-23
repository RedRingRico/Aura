#include <Game.hpp>
#include <PandoraFullscreen.hpp>
#include <iostream>

namespace Aura
{
	AUR_UINT32 Game::PlatformInitialise( )
	{
		if( Aura::EnterPandoraFullscreen( ) != AUR_OK )
		{
			std::cout << "[Aura::Game::PlatformInitialise] <ERROR> "
				"Failed to enter fullscreen mode" << std::endl;

			return AUR_FAIL;
		}

		return AUR_OK;
	}

	AUR_UINT32 Game::PlatformTerminate( )
	{
		Aura::ExitPandoraFullscreen( );

		return AUR_OK;
	}
}

