#include <Gamepad.hpp>

namespace Aura
{
	Gamepad::Gamepad( ) :
		m_ButtonsFD( -1 ),
		m_LeftStickFD( -1 ),
		m_RightStickFD( -1 ),
		m_MaxFD( 0 )
	{
		m_GamepadState.Buttons = 0;
			m_GamepadState.AnalogueStick[ GAMEPAD_STICK_LEFT ].X =
			m_GamepadState.AnalogueStick[ GAMEPAD_STICK_LEFT ].Y =
			m_GamepadState.AnalogueStick[ GAMEPAD_STICK_RIGHT ].X =
			m_GamepadState.AnalogueStick[ GAMEPAD_STICK_RIGHT ].Y = 0.0f;
	}

	Gamepad::~Gamepad( )
	{
		this->Terminate( );
	}
}

