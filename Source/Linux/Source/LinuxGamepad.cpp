#include <Gamepad.hpp>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

namespace Aura
{
	// This will only work for the Xbox 360 controller at the moment
	static AUR_UINT32 s_CodeToButton[ 16 ] =
	{
		// 0
		GAMEPAD_BUTTON_A, GAMEPAD_BUTTON_B, GAMEPAD_BUTTON_X, GAMEPAD_BUTTON_Y,
			GAMEPAD_BUTTON_L1, GAMEPAD_BUTTON_R1, GAMEPAD_BUTTON_SELECT,
			GAMEPAD_BUTTON_START,
		// 8
		GAMEPAD_BUTTON_MENU
	};

	AUR_UINT32 Gamepad::Initialise( )
	{
		this->Terminate( );

		// Just get the first gamepad attached (make sure it has the right
		// amount of buttons and sticks)
		m_ButtonsFD = open( "/dev/input/js0", O_RDONLY | O_NONBLOCK );

		if( m_ButtonsFD < 0 )
		{
			std::cout << "[Aura::Gamepad::Initialise] <ERROR> "
				"Failed to open /dev/input/js0" << std::endl;

			return AUR_FAIL;
		}

		return AUR_OK;
	}

	void Gamepad::Terminate( )
	{
		if( m_ButtonsFD != -1 )
		{
			close( m_ButtonsFD );
			m_ButtonsFD = -1;
		}
	}

	AUR_UINT32 Gamepad::GetState( void *p_pState )
	{
		struct js_event JoyStickEvent;

		while( read( m_ButtonsFD, &JoyStickEvent, sizeof( JoyStickEvent ) ) ==
			sizeof( JoyStickEvent ) )
		{
			JoyStickEvent.type &= ~JS_EVENT_INIT;

			if( JoyStickEvent.type == JS_EVENT_AXIS )
			{
				switch( JoyStickEvent.number )
				{
					// Left stick; X, Y
					case 0:
					{
						m_GamepadState.AnalogueStick[ 0 ].X =
							JoyStickEvent.value / 32767.0f;
						break;
					}
					case 1:
					{
						m_GamepadState.AnalogueStick[ 0 ].Y =
							JoyStickEvent.value / 32767.0f;
						break;
					}
					// Right stick; X, Y
					//
					case 3:
					{
						m_GamepadState.AnalogueStick[ 1 ].X =
							JoyStickEvent.value / 32767.0f;
						break;
					}
					case 4:
					{
						m_GamepadState.AnalogueStick[ 1 ].Y =
							JoyStickEvent.value / 32767.0f;
						break;
					}
					// D-Pad, Left/Right, Up/Down
					case 6:
					{
						m_GamepadState.Buttons &=
							~( GAMEPAD_BUTTON_DPAD_LEFT |
								GAMEPAD_BUTTON_DPAD_RIGHT );
							
						if( JoyStickEvent.value < 0 )
						{
							m_GamepadState.Buttons |=
								GAMEPAD_BUTTON_DPAD_LEFT;
						}
						else if( JoyStickEvent.value > 0 )
						{
							m_GamepadState.Buttons |=
								GAMEPAD_BUTTON_DPAD_RIGHT;
						}
						break;
					}
					case 7:
					{
						m_GamepadState.Buttons &=
							~( GAMEPAD_BUTTON_DPAD_UP |
								GAMEPAD_BUTTON_DPAD_DOWN );
						if( JoyStickEvent.value < 0 )
						{
							m_GamepadState.Buttons |=
								GAMEPAD_BUTTON_DPAD_UP;
						}
						else if( JoyStickEvent.value > 0 )
						{
							m_GamepadState.Buttons |=
								GAMEPAD_BUTTON_DPAD_DOWN;
						}
						break;
					}
				}
			}

			if( JoyStickEvent.type == JS_EVENT_BUTTON )
			{
				if( JoyStickEvent.value )
				{
					m_GamepadState.Buttons |=
						s_CodeToButton[ JoyStickEvent.number ];
				}
				else
				{
					m_GamepadState.Buttons  &=
						~( s_CodeToButton[ JoyStickEvent.number ] );
				}
			}
		}

		memcpy( p_pState, &m_GamepadState, sizeof( m_GamepadState ) );

		return AUR_OK;
	}
}

