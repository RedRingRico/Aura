#ifndef __AURA_TEXTURE_HPP__
#define __AURA_TEXTURE_HPP__

#include <DataTypes.hpp>
#include <GLES2/gl2.h>
#include <string>

namespace Aura
{
#pragma pack( 1 )
    struct TARGA_HEADER
    {
		AUR_BYTE		IDLength;
		AUR_BYTE		ColourmapType;
		AUR_BYTE		ImageType;
		AUR_BYTE		ColourmapSpecification[ 5 ];
		AUR_UINT16		X;
		AUR_UINT16		Y;
		AUR_UINT16		Width;
		AUR_UINT16		Height;
		AUR_BYTE		BitsPerPixel;
		AUR_BYTE		ImageDescription;
    };
#pragma pack( )

	class Texture
	{
	public:
		Texture( );
		~Texture( );

		AUR_UINT32 LoadFromFile( const std::string &p_FileName );

		void Destroy( );

		AUR_UINT32 Activate( );

		AUR_UINT32 SetTextureUnit( const AUR_UINT32 p_TextureUnit );

		AUR_UINT16 GetWidth( ) const;
		AUR_UINT16 GetHeight( ) const;

		AUR_UINT32 GetHash( ) const;

	private:
		Texture( const Texture &p_Other );
		Texture &operator=( const Texture &p_Other );

		AUR_UINT16	m_Width;
		AUR_UINT16	m_Height;
		GLuint		m_TextureID;
		GLenum		m_TextureUnit;
		GLenum		m_TextureType;
		AUR_UINT32	m_Hash;
	};
}

#endif // __AURA_TEXTURE_HPP__

