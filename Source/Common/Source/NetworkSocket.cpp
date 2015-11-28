#include <NetworkSocket.hpp>
#include <NetworkDevice.hpp>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

namespace Aura
{
	NetworkSocket::NetworkSocket( ) :
		m_Socket( -1 )
	{
	}

	NetworkSocket::~NetworkSocket( )
	{
		this->Close( );
	}

	AUR_UINT32 NetworkSocket::Open( const NetworkDevice *p_pNetworkDevice,
		const SOCKET_TYPE p_Type, const SOCKET_PROTOCOL p_Protocol )
	{
		// Ignore the network device for now, just use the socket type and
		// protocol specified

		switch( p_Type )
		{
			case SOCKET_TYPE_DGRAM:
			{
				m_SocketType = SOCK_DGRAM;
				break;
			}
			case SOCKET_TYPE_STREAM:
			{
				m_SocketType = SOCK_STREAM;
				break;
			}
			case SOCKET_TYPE_RAW:
			{
				m_SocketType = SOCK_RAW;
				break;
			}
		}

		switch( p_Protocol )
		{
			case SOCKET_PROTOCOL_UDP:
			{
				m_SocketProtocol = IPPROTO_UDP;
				break;
			}
			case SOCKET_PROTOCOL_TCP:
			{
				m_SocketProtocol = IPPROTO_TCP;
				break;
			}
		}

		m_Socket = socket( AF_INET, m_SocketType, m_SocketProtocol );

		if( m_Socket == -1 )
		{
			return AUR_FAIL;
		}

		return AUR_OK;
	}

	void NetworkSocket::Close( )
	{
		if( m_Socket != -1 )
		{
			close( m_Socket );
		}
	}

	AUR_UINT32 NetworkSocket::Connect( const std::string &p_Host,
		const std::string &p_Port )
	{
		struct addrinfo Hints, *pServer;

		memset( &Hints, 0, sizeof( Hints ) );

		Hints.ai_family = AF_INET;
		Hints.ai_socktype = m_SocketType;

		if( getaddrinfo( p_Host.c_str( ), p_Port.c_str( ), &Hints,
			&pServer ) != 0 )
		{
			return AUR_FAIL;
		}

		if( m_SocketType == SOCK_DGRAM )
		{
			m_Server.ai_flags = pServer->ai_flags;
			m_Server.ai_family = pServer->ai_family;
			m_Server.ai_socktype = pServer->ai_socktype;
			m_Server.ai_protocol = pServer->ai_protocol;
			m_Server.ai_addrlen = pServer->ai_addrlen;
			m_Server.ai_addr = new struct sockaddr;
			memcpy( m_Server.ai_addr, pServer->ai_addr,
				sizeof( struct sockaddr ) );
			/*m_Server.ai_canonname =
				new char[ strlen( pServer->ai_canonname ) + 1 ];
			strncpy( m_Server.ai_canonname, pServer->ai_canonname,
				strlen( pServer->ai_canonname ) );
			m_Server.ai_canonname[ strlen( pServer->ai_canonname ) ] = '\0';*/
			//m_Server.ai_next = AUR_NULL;
		}

		freeaddrinfo( pServer );

		return AUR_OK;
	}

	void NetworkSocket::Send( const AUR_BYTE *p_pData,
		const AUR_UINT32 p_Size )
	{
		if( m_SocketType == SOCK_DGRAM )
		{
			if( sendto( m_Socket, p_pData, p_Size, 0, m_Server.ai_addr,
				m_Server.ai_addrlen ) == -1 )
			{
				std::cout << "[Aura::NetworkSocket::Send] <ERROR> "
					"Failed to send data" << std::endl;
			}
		}
	}
}

