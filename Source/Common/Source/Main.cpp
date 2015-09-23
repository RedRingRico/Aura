#include <iostream>
#include <GitVersion.hpp>
#include <PandoraFullscreen.hpp>
#include <Window.hpp>
#include <unistd.h>
#include <GLES2/gl2.h>
#include <Game.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Aura" << std::endl;
	std::cout << "Build Information" << std::endl;
	std::cout << "\tDate: " << GIT_COMMITTERDATE << std::endl;
	std::cout << "\tVersion: " << GIT_BUILD_VERSION << std::endl;
	std::cout << "\tBranch: " << GIT_BRANCH << std::endl;
	std::cout << "\tTag: " << GIT_TAG_NAME << std::endl;

	Aura::Game AuraGame;

	if( AuraGame.Initialise( ) != AUR_OK )
	{
		std::cout << "[main] <ERROR> Failed to initialise the game" <<
			std::endl;

		return AUR_FAIL;
	}

	return AuraGame.Execute( );
}

