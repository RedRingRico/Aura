#include <Font.hpp>
#include <RendererPrimitive.hpp>
#include <iostream>
#include <cstdarg>
#include <cstdio>

namespace Aura
{
	Font::Font( )
	{
	}

	Font::~Font( )
	{
	}

	AUR_UINT32 Font::LoadTextureFromFile( const std::string &p_FileName )
	{
		if( m_Texture.LoadFromFile( p_FileName ) != AUR_OK )
		{
			std::cout << "[Aura::Font::LoadTextureFromFile] <ERROR> "
				"Failed to load texture: \"" << p_FileName << "\"" <<
				std::endl;

			return AUR_FAIL;
		}

		return AUR_OK;
	}

	AUR_UINT32 Font::RenderString( AUR_FLOAT32 p_X, AUR_FLOAT32 p_Y,
		const char *p_pFormat, ... )
	{
		AUR_MEMSIZE StringLength = 0;
		va_list ArgPtr;

		va_start( ArgPtr, p_pFormat );
		StringLength = vsnprintf( AUR_NULL, 0, p_pFormat, ArgPtr );
		va_end( ArgPtr );

		++StringLength;

		char CompleteString[ StringLength ];
		AUR_SINT32 Return;

		va_start( ArgPtr, p_pFormat );
		Return = vsnprintf( CompleteString, StringLength, p_pFormat,
			ArgPtr );
		va_end( ArgPtr );

		CompleteString[ StringLength - 1 ] = '\0';

		RendererPrimitive StringPrimitives;

		AUR_FLOAT32 XPosition = 0.0f;

		for( AUR_MEMSIZE Char = 0; Char < StringLength; ++Char )
		{
			auto Glyph = m_CharGlyphMap.find( CompleteString[ Char ] );

			if( Glyph == m_CharGlyphMap.end( ) )
			{
				continue;
			}
		}

		return AUR_OK;
	}
}

