#ifndef __AURA_MEMORY_HPP__
#define __AURA_MEMORY_HPP__

#include <DataTypes.hpp>

namespace Aura
{
	template< typename T >
	void SafeDelete( T *&p_pPointer )
	{
		if( p_pPointer )
		{
			delete p_pPointer;
			p_pPointer = AUR_NULL;
		}
	}

	template< typename T >
	void SafeDeleteArray( T *&p_pPointer )
	{
		if( p_pPointer )
		{
			delete [ ] p_pPointer;
			p_pPointer = AUR_NULL;
		}
	}
}

#endif // __AURA_MEMORY_HPP__

