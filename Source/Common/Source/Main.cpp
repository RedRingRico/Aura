#include <iostream>
#include <GitVersion.hpp>
#include <PandoraFullscreen.hpp>
#include <Window.hpp>
#include <unistd.h>
#include <GLES2/gl2.h>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Aura" << std::endl;
	std::cout << "Build Information" << std::endl;
	std::cout << "\tDate: " << GIT_COMMITTERDATE << std::endl;
	std::cout << "\tVersion: " << GIT_BUILD_VERSION << std::endl;
	std::cout << "\tBranch: " << GIT_BRANCH << std::endl;
	std::cout << "\tTag: " << GIT_TAG_NAME << std::endl;

	Aura::EnterPandoraFullscreen( );
	Aura::Window GameWindow;

	if( GameWindow.Create( ) != AUR_OK )
	{
		Aura::ExitPandoraFullscreen( );
		return 1;
	}

	glClearColor( 1.0f, 1.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	sleep( 4 );

	GameWindow.Destroy( );

	Aura::ExitPandoraFullscreen( );

	return 0;
}

