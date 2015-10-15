#include <FreeTimer.hpp>
#include <cstring>
#include <iostream>
#include <time.h>

namespace Aura
{
	FreeTimer::FreeTimer( )	:
		m_StartTime( 0ULL ),
		m_ElapsedTime( 0ULL ),
		m_Running( AUR_FALSE ),
		m_Stopped( AUR_FALSE ),
		m_Paused( AUR_FALSE ),
		m_TimerLock( PTHREAD_MUTEX_INITIALIZER ),
		m_TimerCond( PTHREAD_COND_INITIALIZER )
	{
	}

	FreeTimer::~FreeTimer( )
	{
	}

	AUR_UINT32 FreeTimer::Start( )
	{
		pthread_mutex_lock( &m_TimerLock );
		struct timespec TimeSpec;

		clock_gettime( CLOCK_REALTIME, &TimeSpec );

		AUR_UINT64 Second, Microsecond;
		Second = TimeSpec.tv_sec * 1000000ULL;
		Microsecond = TimeSpec.tv_nsec / 1000ULL;

		m_StartTime = Second + Microsecond;

		m_ElapsedTime = 0ULL;

		int Error = pthread_create( &m_ThreadID, AUR_NULL, this->Run,
			this );
		
		if( Error != 0 )
		{
			std::cout << "[Aura::FreeTimer::Start] <ERROR> "
				"Failed to create thread" << std::endl;
			return AUR_FAIL;
		}

		pthread_cond_wait( &m_TimerCond, &m_TimerLock );
		pthread_mutex_unlock( &m_TimerLock );

		m_Running = AUR_TRUE;

		return AUR_OK;
	}

	AUR_UINT32 FreeTimer::Stop( )
	{
		pthread_cancel( m_ThreadID );
		pthread_join( m_ThreadID, AUR_NULL );

		m_Stopped = AUR_TRUE;

		return AUR_OK;
	}

	AUR_UINT32 FreeTimer::Pause( )
	{
		pthread_cancel( m_ThreadID );
		pthread_join( m_ThreadID, AUR_NULL );

		m_Paused = AUR_TRUE;

		return AUR_OK;
	}

	AUR_UINT32 FreeTimer::Resume( )
	{
		if( m_Paused )
		{
			pthread_mutex_lock( &m_TimerLock );
			struct timespec TimeSpec;

			clock_gettime( CLOCK_REALTIME, &TimeSpec );

			AUR_UINT64 Second, Microsecond;
			Second = TimeSpec.tv_sec * 1000000ULL;
			Microsecond = TimeSpec.tv_nsec / 1000ULL;

			m_StartTime = Second + Microsecond;

			int Error = pthread_create( &m_ThreadID, AUR_NULL, this->Run,
				this );
			
			if( Error != 0 )
			{
				std::cout << "[Aura::FreeTimer::Pause] <ERROR> "
					"Failed to create thread" << std::endl;

				return AUR_FAIL;
			}

			pthread_cond_wait( &m_TimerCond, &m_TimerLock );
			pthread_mutex_unlock( &m_TimerLock );

			m_Paused = AUR_FALSE;
		}
		else
		{
			return this->Start( );
		}

		return AUR_OK;
	}

	AUR_BOOL FreeTimer::IsRunning( ) const
	{
		return m_Running;
	}

	AUR_BOOL FreeTimer::IsStopped( ) const
	{
		return m_Stopped;
	}

	AUR_BOOL FreeTimer::IsPaused( ) const
	{
		return m_Paused;
	}

	AUR_UINT64 FreeTimer::GetSeconds( ) const
	{
		return m_ElapsedTime / 1000000ULL;
	}

	AUR_UINT64 FreeTimer::GetMilliseconds( ) const
	{
		return m_ElapsedTime / 1000ULL;
	}

	AUR_UINT64 FreeTimer::GetMicroseconds( ) const
	{
		return m_ElapsedTime;
	}

	void *FreeTimer::Run( void *p_pArg )
	{
		FreeTimer *pThisInst = static_cast< FreeTimer * >( p_pArg );

		struct timespec TimeSpec;

		pthread_mutex_lock( &pThisInst->m_TimerLock );
		pthread_cond_signal( &pThisInst->m_TimerCond );
		pthread_mutex_unlock( &pThisInst->m_TimerLock );

		AUR_UINT64 PreviousTime = 0ULL;
		AUR_UINT64 DeltaTime = 0ULL;

		clock_gettime( CLOCK_REALTIME, &TimeSpec );
		AUR_UINT64 Second = TimeSpec.tv_sec * 1000000ULL;
		AUR_UINT64 Microsecond = TimeSpec.tv_nsec / 1000ULL;
		PreviousTime = Second + Microsecond;

		while( 1 )
		{
			pthread_testcancel( );
			clock_gettime( CLOCK_REALTIME, &TimeSpec );
			Second = TimeSpec.tv_sec * 1000000ULL;
			Microsecond = TimeSpec.tv_nsec / 1000ULL;

			DeltaTime = ( Second + Microsecond ) - PreviousTime;

			PreviousTime = Second + Microsecond;

			pThisInst->m_ElapsedTime += DeltaTime;
		}

		return AUR_NULL;
	}
}

