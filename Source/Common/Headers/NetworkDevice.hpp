#ifndef __AURA_NETWORKDEVICE_HPP__
#define __AURA_NETWORKDEVICE_HPP__

#include <DataTypes.hpp>

namespace Aura
{
	class NetworkDevice
	{
	public:
		NetworkDevice( );
		~NetworkDevice( );

	private:
	};

	AUR_UINT32 EnumerateNetworkDevices( NetworkDevice **p_ppNetworkDevices );
}

#endif // __AURA_NETWORKDEVICE_HPP__

