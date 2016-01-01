#ifndef __AURA_CHATNETWORKMESSAGE_HPP__
#define __AURA_CHATNETWORKMESSAGE_HPP__

#include <DataTypes.hpp>
#include <NetworkMessage.hpp>
#include <string>

namespace Aura
{
#pragma pack( 1 )
    class ChatNetworkMessage : public NetworkMessage
    {
	public:
		ChatNetworkMessage( );
		virtual ~ChatNetworkMessage( );

		void SetChatMessage( const AUR_UINT32 p_Sender,
			const AUR_UINT32 p_Receiver, const std::string p_Message );

	private:
		AUR_UINT32	m_Sender;
		AUR_UINT32	m_Receiver;
		char		m_Message[ NET_MESSAGE_MAX_LENGTH -
						sizeof( NETWORK_MESSAGE_HEADER ) -
						( sizeof( AUR_UINT32 ) * 2 ) ];
    };
#pragma pack( 0 )
}

#endif // __AURA_CHATNETWORKMESSAGE_HPP__

