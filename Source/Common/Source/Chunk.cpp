#include <Chunk.hpp>
#include <iostream>

namespace Aura
{
	AUR_UINT32 ReadChunk( CHUNK &p_Chunk, FILE *p_pFile )
	{
		if( fread( &p_Chunk, sizeof( p_Chunk ), 1, p_pFile ) != 1 )
		{
			std::cout << "[Aura::ReadChunk] <ERROR> "
				"Failed to read chunk" << std::endl;

			return AUR_FAIL;
		}

		return AUR_OK;
	}
}

