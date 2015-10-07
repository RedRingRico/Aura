#ifndef __AURA_CHUNK_HPP__
#define __AURA_CHUNK_HPP__

#include <DataTypes.hpp>
#include <cstdio>

namespace Aura
{
#pragma pack( 1 )
	struct CHUNK
	{
		AUR_UINT16	Type;
		AUR_UINT32	Size;
	};
#pragma pack( )

	const AUR_UINT16 CHUNK_END	= 0xFFFF;

	AUR_UINT32 ReadChunk( CHUNK &p_Chunk, FILE *p_pFile );
}

#endif // __AURA_CHUNK_HPP__

