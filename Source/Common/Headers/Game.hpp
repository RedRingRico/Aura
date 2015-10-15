#ifndef __AURA_GAME_HPP__
#define __AURA_GAME_HPP__

#include <DataTypes.hpp>
#include <Window.hpp>
#include <Renderer.hpp>
#include <Gamepad.hpp>
#include <Font.hpp>

namespace Aura
{
	class Game
	{
	public:
		Game( );
		~Game( );

		AUR_UINT32 Initialise( );
		AUR_UINT32 Execute( );

	private:
		AUR_UINT32 PlatformInitialise( );
		AUR_UINT32 PlatformTerminate( );

		Window		m_Window;
		Renderer	m_Renderer;
		Gamepad		m_Gamepad;
		Font		*m_pTestFont;
	};
}

#endif // __AURA_GAME_HPP__

