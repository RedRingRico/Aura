#ifndef __AURA_INPUTDEVICE_HPP__
#define __AURA_INPUTDEVICE_HPP__

#include <DataTypes.hpp>

namespace Aura
{
	enum INPUT_DEVICE_TYPE
	{
		INPUT_DEVICE_TYPE_KEYBOARD,
		INPUT_DEVICE_TYPE_GAMEPAD,
		INPUT_DEVICE_TYPE_UNKNOWN
	};

	struct AXIS_2D
	{
		AUR_FLOAT32 X;
		AUR_FLOAT32 Y;
	};

	const AUR_UINT32 GAMEPAD_BUTTON_DPAD_UP		= 0x00000001;
	const AUR_UINT32 GAMEPAD_BUTTON_DPAD_DOWN	= 0x00000002;
	const AUR_UINT32 GAMEPAD_BUTTON_DPAD_LEFT	= 0x00000004;
	const AUR_UINT32 GAMEPAD_BUTTON_DPAD_RIGHT	= 0x00000008;
	const AUR_UINT32 GAMEPAD_BUTTON_A			= 0x00000010;
	const AUR_UINT32 GAMEPAD_BUTTON_B			= 0x00000020;
	const AUR_UINT32 GAMEPAD_BUTTON_X			= 0x00000040;
	const AUR_UINT32 GAMEPAD_BUTTON_Y			= 0x00000080;
	const AUR_UINT32 GAMEPAD_BUTTON_START		= 0x00000100;
	const AUR_UINT32 GAMEPAD_BUTTON_SELECT		= 0x00000200;
	const AUR_UINT32 GAMEPAD_BUTTON_MENU		= 0x00000400;
	const AUR_UINT32 GAMEPAD_BUTTON_L1			= 0x00001000;
	const AUR_UINT32 GAMEPAD_BUTTON_L2			= 0x00002000;
	const AUR_UINT32 GAMEPAD_BUTTON_R1			= 0x00004000;
	const AUR_UINT32 GAMEPAD_BUTTON_R2			= 0x00008000;

	const AUR_UINT32 GAMEPAD_STICK_LEFT		= 0;
	const AUR_UINT32 GAMEPAD_STICK_RIGHT	= 1;

	struct GAMEPAD_STATE
	{
		AUR_UINT32	Buttons;
		AXIS_2D		AnalogueStick[ 2 ];
	};

	class InputDevice
	{
	public:
		virtual ~InputDevice( );

		virtual AUR_UINT32 Initialise( ) = 0;
		virtual void Terminate( ) = 0;

		virtual AUR_UINT32 GetState( void *p_pState ) = 0;

		INPUT_DEVICE_TYPE GetType( ) const;

	protected:
		INPUT_DEVICE_TYPE	m_DeviceType;
	};
}

#endif // __AURA_INPUTDEVICE_HPP__
