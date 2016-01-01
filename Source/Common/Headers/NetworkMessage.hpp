#ifndef __AURA_NETWORKMESSAGE_HPP__
#define __AURA_NETWORKMESSAGE_HPP__

#include <DataTypes.hpp>

namespace Aura
{

	const AUR_UINT32 NET_MESSAGE_CONNECT	= 1;
	const AUR_UINT32 NET_MESSAGE_DISCONNECT	= 2;

	const AUR_UINT32 NET_MESSAGE_PLAYER_JOINED	= 100;
	const AUR_UINT32 NET_MESSAGE_PLAYER_LEFT	= 101;
	const AUR_UINT32 NET_MESSAGE_PLAYER_INFO	= 102;

	const AUR_UINT32 NET_MESSAGE_CHAT		 	= 200;

#pragma pack ( 1 )
	struct NETWORK_MESSAGE_HEADER
	{
		// Network message ID (Connect, Disconnect, Player Joined, ...)
		AUR_UINT32	ID;
		// Length covers the data section, not the header, add
		// sizeof( NetworkMessageHeader ) to get the complete length
		AUR_UINT32	Length;
		// The time (in microseconds) that this message was received
		AUR_UINT64	Time;
	};

	const AUR_UINT32 NET_MESSAGE_MAX_LENGTH = 1400;

	class NetworkMessage
	{
	public:
		NetworkMessage( );
		virtual ~NetworkMessage( );

		// This returns the overall length of the message
		AUR_UINT32 GetLength( ) const;

		virtual void *GetData( );

	protected:
		NETWORK_MESSAGE_HEADER	m_Header;

		AUR_BYTE m_Data[ NET_MESSAGE_MAX_LENGTH ];
	};

#pragma pack( 0 )
}

#endif // __AURA_NETWORKMESSAGE_HPP__

