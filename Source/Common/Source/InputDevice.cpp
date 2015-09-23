#include <InputDevice.hpp>

namespace Aura
{
	InputDevice::~InputDevice( )
	{
	}

	INPUT_DEVICE_TYPE InputDevice::GetType( ) const
	{
		return m_DeviceType;
	}
}

