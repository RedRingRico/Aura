#include <Gamepad.hpp>
#include <iostream>
#include <cstring>
#include <sys/time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>

namespace Aura
{
	static AUR_UINT32 s_CodeToButton[ 256 ] =
	{
		// 0
		0, 0, 0, 0, 0, 0, 0, 0,
		// 8
		0, 0, 0, 0, 0, 0, 0, 0,
		// 16
		0, 0, 0, 0, 0, 0, 0, 0,
		// 24
		0, 0, 0, 0, 0, GAMEPAD_BUTTON_SELECT, 0, 0,
		// 32
		0, 0, 0, 0, 0, 0, 0, 0,
		// 40
		0, 0, 0, 0, 0, 0, 0, 0,
		// 48
		0, 0, 0, 0, 0, 0, GAMEPAD_BUTTON_L1, 0,
		// 56
		GAMEPAD_BUTTON_START, 0, 0, 0, 0, 0, 0, 0,
		// 64
		0, 0, 0, 0, 0, 0, 0, 0,
		// 72
		0, 0, 0, 0, 0, 0, 0, 0,
		// 80
		0, 0, 0, 0, 0, 0, 0, 0,
		// 88
		0, 0, 0, 0, 0, 0, 0, 0,
		// 96
		0, GAMEPAD_BUTTON_R1, 0, 0, 0, 0, GAMEPAD_BUTTON_A,
			GAMEPAD_BUTTON_DPAD_UP,
		// 104
		GAMEPAD_BUTTON_Y, GAMEPAD_BUTTON_DPAD_LEFT, GAMEPAD_BUTTON_DPAD_RIGHT,
			GAMEPAD_BUTTON_B, GAMEPAD_BUTTON_DPAD_DOWN, GAMEPAD_BUTTON_X, 0, 0,
		// 112
		0, 0, 0, 0, 0, 0, 0, 0,
		// 120
		0, 0, 0, 0, 0, 0, 0, 0,
		// 128
		0, 0, 0, 0, 0, 0, 0, 0,
		// 136
		0, 0, 0, GAMEPAD_BUTTON_MENU, 0, 0, 0, 0,
	};

	AUR_UINT32 Gamepad::Initialise( )
	{
		this->Terminate( );

		m_MaxFD = 0;

		for( int Index = 0; ; ++Index )
		{
			char DeviceName[ 64 ];
			char PrintName[ 256 ];
			int FD;

			snprintf( DeviceName, sizeof( DeviceName ), "/dev/input/event%i",
				Index );

			FD = open( DeviceName, O_RDONLY );

			// Done, no more event interfaces exist
			if( FD == -1 )
			{
				break;
			}

			ioctl( FD, EVIOCGNAME( sizeof( PrintName ) ), PrintName );

			if( strcmp( PrintName, "gpio-keys" ) == 0 )
			{
				m_ButtonsFD = FD;
			}
			else if( strcmp( PrintName, "nub0" ) == 0 )
			{
				m_LeftStickFD = FD;
			}
			else if( strcmp( PrintName, "nub1" ) == 0 )
			{
				m_RightStickFD = FD;
			}

			if( FD > m_MaxFD )
			{
				m_MaxFD = FD;
			}
		}

		AUR_UINT32 Return = AUR_OK;

		if( m_ButtonsFD == -1 )
		{
			std::cout << "[Aura::Gamepad::Intialise] <ERROR> "
				"Failed to open gamepad buttons for reading" << std::endl;

			Return = AUR_FAIL;
		}

		if( m_LeftStickFD == -1 )
		{
			std::cout << "[Aura::Gamepad::Intialise] <ERROR> "
				"Failed to open left analogue stick for reading" << std::endl;

			Return = AUR_FAIL;
		}

		if( m_RightStickFD == -1 )
		{
			std::cout << "[Aura::Gamepad::Intialise] <ERROR> "
				"Failed to open right analogue for reading" << std::endl;

			Return = AUR_FAIL;
		}

		return Return;
	}

	void Gamepad::Terminate( )
	{
		if( m_ButtonsFD != -1 )
		{
			close( m_ButtonsFD );
			m_ButtonsFD = -1;
		}

		if( m_LeftStickFD != -1 )
		
		{
			close( m_LeftStickFD );
			m_LeftStickFD = -1;
		}

		if( m_RightStickFD != -1 )
		{
			close( m_RightStickFD );
			m_RightStickFD = -1;
		}
	}

	AUR_UINT32 Gamepad::GetState( void *p_pState )
	{
		if( p_pState == AUR_NULL )
		{
			std::cout << "[Aura::Gamepad::GetState] <ERROR> "
				"State parameter null" << std::endl;

			return AUR_FAIL;
		}

		struct input_event Event[ 64 ];
		int FD = -1, Read, Return;
		fd_set ReadFDS;

		FD_ZERO( &ReadFDS );

		FD_SET( m_ButtonsFD, &ReadFDS );
		FD_SET( m_LeftStickFD, &ReadFDS );
		FD_SET( m_RightStickFD, &ReadFDS );

		// Return immediately
		struct timeval TimeOut = { 0, 0 };

		Return = select( m_MaxFD + 1, &ReadFDS, AUR_NULL, AUR_NULL, &TimeOut );

		if( Return == -1 )
		{
			std::cout << "[Aura::Gamepad::GetState] <ERROR> "
				"Failed to call select" << std::endl;

			return AUR_FAIL;
		}

		if( FD_ISSET( m_ButtonsFD, &ReadFDS ) )
		{
			FD = m_ButtonsFD;
		}

		if( FD_ISSET( m_LeftStickFD, &ReadFDS ) )
		{
			FD = m_LeftStickFD;
		}

		if( FD_ISSET( m_RightStickFD, &ReadFDS ) )
		{
			FD = m_RightStickFD;
		}

		Read = read( FD, Event, sizeof( Event ) );

		if( Read < ( int )( sizeof( Event[ 0 ] ) ) )
		{
			memcpy( p_pState, &m_GamepadState, sizeof( m_GamepadState ) );

			return AUR_OK;
		}

		for( AUR_MEMSIZE i = 0; i < Read / sizeof( Event[ 0 ] ); ++i )
		{
			switch( Event[ i ].type )
			{
				case EV_ABS:
				{
					int Stick = ( FD == m_LeftStickFD ) ? GAMEPAD_STICK_LEFT :
						GAMEPAD_STICK_RIGHT;

					if( Event[ i ].code == ABS_X )
					{
						m_GamepadState.AnalogueStick[ Stick ].X ==
							Event[ i ].value / 256.0f;
					}
					if( Event[ i ].code == ABS_Y )
					{
						m_GamepadState.AnalogueStick[ Stick ].Y ==
							Event[ i ].value / 256.0f;
					}
					break;
				}
				case EV_KEY:
				{
					if( Event[ i ].value )
					{
						m_GamepadState.Buttons |=
							s_CodeToButton[ Event[ i ].code ];
					}
					else
					{
						m_GamepadState.Buttons &=
							~( s_CodeToButton[ Event[ i ].code ] );
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}

		memcpy( p_pState, &m_GamepadState, sizeof( m_GamepadState ) );

		return AUR_OK;
	}
}

