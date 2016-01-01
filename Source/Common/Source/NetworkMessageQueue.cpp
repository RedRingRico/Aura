#include <NetworkMessageQueue.hpp>
#include <iostream>

namespace Aura
{
	NetworkMessageQueue::NetworkMessageQueue( const AUR_MEMSIZE p_QueueSize ) :
		m_Capacity( p_QueueSize ),
		m_Front( 0 ),
		m_Count( 0 )
	{
		m_pQueue = new NETWORK_MESSAGE_QUEUE_ELEMENT[ m_Capacity ];
	}

	NetworkMessageQueue::~NetworkMessageQueue( )
	{
		if( m_pQueue )
		{
			delete [ ] m_pQueue;
		}
	}

	AUR_UINT32 NetworkMessageQueue::Add(
		const NETWORK_MESSAGE_QUEUE_ELEMENT &p_Message )
	{
		if( m_Count == m_Capacity )
		{
			return AUR_FAIL;
		}

		m_pQueue[ m_Count ] = p_Message;

		++m_Count;

		return AUR_OK;
	}

	AUR_UINT32 NetworkMessageQueue::GetFront(
		NETWORK_MESSAGE_QUEUE_ELEMENT &p_Message, const AUR_BOOL p_Dequeue )
	{
		if( m_Count > 0 )
		{
			p_Message = m_pQueue[ m_Front ];

			if( p_Dequeue )
			{
				this->Dequeue( );
			}

			return AUR_OK;
		}

		return AUR_FAIL;
	}

	void NetworkMessageQueue::Dequeue( )
	{
		++m_Front;
		--m_Count;
	}

	AUR_MEMSIZE NetworkMessageQueue::Count( ) const
	{
		return m_Count;
	}
}

