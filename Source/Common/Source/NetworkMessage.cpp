#include <NetworkMessage.hpp>
#include <cstring>

namespace Aura
{
	NetworkMessage::NetworkMessage( )
	{
	}

	NetworkMessage::~NetworkMessage( )
	{
	}

	AUR_UINT32 NetworkMessage::GetLength( ) const
	{
		return m_Header.Length + sizeof( NETWORK_MESSAGE_HEADER );
	}

	void *NetworkMessage::GetData( )
	{
		return m_Data;
	}
}

