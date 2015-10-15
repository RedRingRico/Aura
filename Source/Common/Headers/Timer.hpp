#ifndef __AURA_TIMER_HPP__
#define __AURA_TIMER_HPP__

#include <DataTypes.hpp>
#include <time.h>

namespace Aura
{
	class Timer
	{
	public:
		Timer( );
		~Timer( );

		AUR_UINT64 Start( );
		AUR_UINT64 Stop( );

		AUR_UINT64 GetSeconds( );
		AUR_UINT64 GetMilliseconds( );
		AUR_UINT64 GetMicroseconds( );
		AUR_UINT64 GetNanoseconds( );

	private:
		void UpdateElapsedTime( );

		AUR_BOOL		m_Stopped;
		AUR_UINT64		m_StartTime;
		AUR_UINT64		m_ElapsedTime;
		struct timespec	m_StartTimeSpec;
	};
}

#endif // __AURA_TIMER_HPP__

