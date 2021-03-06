#ifndef __AURA_FONT_HPP__
#define __AURA_FONT_HPP__

#include <DataTypes.hpp>
#include <Texture.hpp>
#include <string>
#include <map>

namespace Aura
{
	struct GLYPHVERTEX
	{
		AUR_FLOAT32	Position[ 3 ];
		AUR_FLOAT32	ST[ 2 ];
	};

	struct GLYPH
	{
		AUR_UINT16	Width;
		AUR_UINT16	Height;
		AUR_UINT16	BearingY;
		AUR_UINT16	XAdvance;
		GLYPHVERTEX	Polygon[ 4 ];
	};

	class Font
	{
	public:
		Font( );
		~Font( );

		AUR_UINT32 LoadTextureFromFile( const std::string &p_FileName );
		AUR_UINT32 LoadGlyphsFromFile( const std::string &p_FileName );

		AUR_UINT32 RenderString( AUR_FLOAT32 p_X, AUR_FLOAT32 p_Y,
			const char *p_pFormat, ... );

	private:
		std::map< char, GLYPH >	m_CharGlyphMap;
		Texture					m_Texture;
		AUR_UINT32				m_Hash;
	};
}

#endif // __AURA_FONT_HPP__

