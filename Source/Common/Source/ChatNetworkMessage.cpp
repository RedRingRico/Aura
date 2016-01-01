#include <ChatNetworkMessage.hpp>
#include <cstring>

namespace Aura
{
    ChatNetworkMessage::ChatNetworkMessage( )
    {
		m_Header.ID = NET_MESSAGE_CHAT;
		m_Header.Time = 0ULL; // Should be something like GetTimeNow( );
    }

    ChatNetworkMessage::~ChatNetworkMessage( )
    {
    }

    void ChatNetworkMessage::SetChatMessage( const AUR_UINT32 p_Sender,
    	const AUR_UINT32 p_Receiver, const std::string p_Message )
    {
		m_Sender = p_Sender;
		m_Receiver = p_Receiver;
		strncpy( m_Message, p_Message.c_str( ), p_Message.size( ) );
		m_Header.Length = sizeof( m_Sender ) + sizeof( m_Receiver ) +
			p_Message.size( );

		AUR_MEMSIZE Offset = 0;

		memcpy( m_Data, &m_Header, sizeof( m_Header ) );
		Offset += sizeof( m_Header );

		memcpy( m_Data + Offset, &m_Sender, sizeof( m_Sender ) );
		Offset += sizeof( m_Sender );

		memcpy( m_Data + Offset, &m_Receiver, sizeof( m_Receiver ) );
		Offset += sizeof( m_Receiver );

		memcpy( m_Data + Offset, m_Message, p_Message.size( ) );
    }

}

