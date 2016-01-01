#ifndef __AURA_NETWORKSOCKET_HPP__
#define __AURA_NETWORKSOCKET_HPP__

#include <DataTypes.hpp>
#include <NetworkMessageQueue.hpp>
#include <netdb.h>
#include <string>

namespace Aura
{
	enum SOCKET_TYPE
	{
		SOCKET_TYPE_STREAM,
		SOCKET_TYPE_DGRAM,
		SOCKET_TYPE_RAW
	};

	enum SOCKET_PROTOCOL
	{
		SOCKET_PROTOCOL_TCP,
		SOCKET_PROTOCOL_UDP,
		SOCKET_PROTOCOL_IPV4,
		SOCKET_PROTOCOL_IPV6
	};

	class NetworkDevice;

	class NetworkSocket
	{
	public:
		explicit NetworkSocket( const AUR_MEMSIZE p_InboundCapacity,
			const AUR_MEMSIZE p_OutboundCapacity );
		~NetworkSocket( );

		AUR_UINT32 Open( const NetworkDevice *p_pNetworkDevice,
			const SOCKET_TYPE p_Type, const SOCKET_PROTOCOL p_Protocol );
		void Close( );

		AUR_UINT32 Connect( const std::string &p_Host,
			const std::string &p_Port );

		// This will need to be converted to send the queue instead of bytes
		void Send( NetworkMessage *p_pMessage );

		// This must be called to send and receive messages
		void ProcessMessages( );

	private:
		int					m_Socket;
		int					m_SocketType;
		int					m_SocketProtocol;
		struct addrinfo		m_Server;
		NetworkMessageQueue	*m_pInboundQueue;
		NetworkMessageQueue	*m_pOutboundQueue;
	};
}

#endif // __AURA_NETWORKSOCKET_HPP__

