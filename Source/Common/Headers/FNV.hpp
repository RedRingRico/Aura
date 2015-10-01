#ifndef __AURA_FNV_HPP__
#define __AURA_FNV_HPP__

#include <DataTypes.hpp>

namespace Aura
{
	const AUR_UINT32 FNV32_PRIME	= 0x01000193;
	const AUR_UINT32 FNV32_OFFSET	= 0x811C9DC5;

	AUR_UINT32 HashBufferFNV1a( void *p_pBuffer, const AUR_MEMSIZE p_Length,
		AUR_UINT32 p_PreviousHash = FNV32_OFFSET );
	AUR_UINT32 HashStringFNV1a( const char *p_pString,
		AUR_UINT32 p_PreviousHash = FNV32_OFFSET );
}

#endif // __AURA_FNV_HPP__

