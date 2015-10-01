#include <FNV.hpp>

namespace Aura
{
	AUR_UINT32 HashBufferFNV1a( void *p_pBuffer, const AUR_MEMSIZE p_Length,
		AUR_UINT32 p_PreviousHash )
	{
		AUR_BYTE *pStart = static_cast< AUR_BYTE * >( p_pBuffer );
		AUR_BYTE *pEnd = pStart + p_Length;

		while( pStart < pEnd )
		{
			p_PreviousHash ^= static_cast< AUR_UINT32 >( *pStart++ );
			p_PreviousHash *= FNV32_PRIME;
		}

		return p_PreviousHash;
	}

	AUR_UINT32 HashStringFNV1a( const char *p_pString,
		AUR_UINT32 p_PreviousHash )
	{
		const AUR_BYTE *pString =
			reinterpret_cast< const AUR_BYTE * >( p_pString );

		while( *pString )
		{
			p_PreviousHash ^= static_cast< AUR_UINT32 >( *pString++ );
			p_PreviousHash *= FNV32_PRIME;
		}

		return p_PreviousHash;
	}
}

