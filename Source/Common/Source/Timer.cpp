#include <Timer.hpp>
#include <iostream>
#include <cstring>

namespace Aura
{
	struct timespec TimeDifference( struct timespec &p_Start,
		struct timespec &p_End )
	{
		struct timespec Difference;

		if( ( p_End.tv_nsec - p_Start.tv_nsec ) < 0 )
		{
			Difference.tv_sec = p_End.tv_sec - p_Start.tv_sec - 1;
			Difference.tv_nsec = 1000000000 + p_End.tv_nsec -
				p_Start.tv_nsec;
		}
		else
		{
			Difference.tv_sec = p_End.tv_sec - p_Start.tv_sec;
			Difference.tv_nsec = p_End.tv_nsec - p_Start.tv_nsec;
		}

		return Difference;
	}

	Timer::Timer( ) :
		m_Stopped( AUR_TRUE ),
		m_StartTime( 0ULL ),
		m_ElapsedTime( 0ULL )
	{
		memset( &m_StartTimeSpec, 0, sizeof( m_StartTimeSpec ) );
	}

	Timer::~Timer( )
	{
	}

	AUR_UINT64 Timer::Start( )
	{
		clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &m_StartTimeSpec );

		AUR_UINT64 Second = m_StartTimeSpec.tv_sec * 1000000000;
		AUR_UINT64 Nanosecond = m_StartTimeSpec.tv_nsec;

		m_StartTime = Second + Nanosecond;
		m_ElapsedTime = 0ULL;

		m_Stopped = AUR_FALSE;

		return AUR_OK;
	}

	AUR_UINT64 Timer::Stop( )
	{
		if( m_StartTime == 0ULL )
		{
			return AUR_FAIL;
		}

		this->UpdateElapsedTime( );

		m_Stopped = AUR_TRUE;

		return AUR_OK;
	}

	AUR_UINT64 Timer::GetSeconds( )
	{
		if( m_Stopped )
		{
			return m_ElapsedTime / 1000000000ULL;
		}

		this->UpdateElapsedTime( );

		return m_ElapsedTime / 1000000000ULL;
	}

	AUR_UINT64 Timer::GetMilliseconds( )
	{
		if( m_Stopped )
		{
			return m_ElapsedTime / 1000000ULL;
		}

		this->UpdateElapsedTime( );

		return m_ElapsedTime / 1000000ULL;
	}

	AUR_UINT64 Timer::GetMicroseconds( )
	{
		if( m_Stopped )
		{
			return m_ElapsedTime / 1000ULL;
		}

		this->UpdateElapsedTime( );

		return m_ElapsedTime / 1000ULL;
	}

	AUR_UINT64 Timer::GetNanoseconds( )
	{
		if( m_Stopped )
		{
			return m_ElapsedTime;
		}
	
		this->UpdateElapsedTime( );

		return m_ElapsedTime;
	}

	void Timer::UpdateElapsedTime( )
	{
		struct timespec TimeSpec;
		clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &TimeSpec );

		struct timespec Difference = TimeDifference( m_StartTimeSpec,
			TimeSpec );

		AUR_UINT64 Second = Difference.tv_sec * 1000000000ULL;
		AUR_UINT64 Nanosecond = Difference.tv_nsec;

		m_ElapsedTime = Second + Nanosecond;
		/*std::cout << "Second:     " << Second << std::endl;
		std::cout << "Nanosecond: " << Nanosecond << std::endl;
		std::cout << "Elapsed:    " << m_ElapsedTime << std::endl;*/
	}
}

