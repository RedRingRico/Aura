#ifndef __AURA_FREETIMER_HPP__
#define __AURA_FREETIMER_HPP__

#include <DataTypes.hpp>
#include <pthread.h>

namespace Aura
{
	class FreeTimer
	{
	public:
		FreeTimer( );
		~FreeTimer( );

		AUR_UINT32 Start( );
		AUR_UINT32 Stop( );
		AUR_UINT32 Pause( );
		AUR_UINT32 Resume( );

		AUR_BOOL IsRunning( ) const;
		AUR_BOOL IsStopped( ) const;
		AUR_BOOL IsPaused( ) const;

		AUR_UINT64 GetSeconds( ) const;
		AUR_UINT64 GetMilliseconds( ) const;
		AUR_UINT64 GetMicroseconds( ) const;

	private:
		AUR_UINT64		m_StartTime;
		AUR_UINT64		m_ElapsedTime;
		AUR_BOOL		m_Running;
		AUR_BOOL		m_Stopped;
		AUR_BOOL		m_Paused;
		pthread_t		m_ThreadID;
		pthread_mutex_t	m_TimerLock;
		pthread_cond_t	m_TimerCond;

		static void *Run( void *p_pArg );
	};
}

#endif // __AURA_FREETIMER_HPP__

