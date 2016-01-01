#ifndef __AURA_NETWORKMESSAGEQUEUE_HPP__
#define __AURA_NETWORKMESSAGEQUEUE_HPP__

#include <DataTypes.hpp>
#include <NetworkMessage.hpp>

namespace Aura
{
	struct NETWORK_MESSAGE_QUEUE_ELEMENT
	{
		NetworkMessage					Message;
	};

	class NetworkMessageQueue
	{
	public:
		explicit NetworkMessageQueue( const AUR_MEMSIZE p_QueueSize );
		~NetworkMessageQueue( );

		AUR_UINT32 Add( const NETWORK_MESSAGE_QUEUE_ELEMENT &p_Message );
		AUR_UINT32 GetFront( NETWORK_MESSAGE_QUEUE_ELEMENT &p_Message,
			const AUR_BOOL p_Dequeue );
		void Dequeue( );

		AUR_MEMSIZE Count( ) const;

	private:
		AUR_MEMSIZE						m_Capacity;
		AUR_MEMSIZE						m_Front;
		AUR_MEMSIZE						m_Count;
		NETWORK_MESSAGE_QUEUE_ELEMENT	*m_pQueue;
	};
}

#endif // __AURA_NETWORKMESSAGEQUEUE_HPP__

