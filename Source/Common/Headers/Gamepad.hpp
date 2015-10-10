#ifndef __AURA_GAMEPAD_HPP__
#define __AURA_GAMEPAD_HPP__

#include <InputDevice.hpp>

namespace Aura
{
	const AUR_FLOAT32 GAMEPAD_DEADZONE = 0.2f;

	class Gamepad : public InputDevice
	{
	public:
		Gamepad( );
		virtual ~Gamepad( );

		virtual AUR_UINT32 Initialise( );
		virtual void Terminate( );

		virtual AUR_UINT32 GetState( void *p_pState );

	private:
		GAMEPAD_STATE	m_GamepadState;
		int				m_ButtonsFD;
		int				m_LeftStickFD;
		int				m_RightStickFD;
		int				m_MaxFD;
	};
}

#endif // __AURA_GAMEPAD_HPP__

