#include <Texture.hpp>
#include <FNV.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstring>
#include <GLES2/gl2ext.h>

namespace Aura
{
    Texture::Texture( ) :
    	m_Width( 0 ),
		m_Height( 0 ),
		m_TextureID( 0 ),
		m_TextureUnit( 0 ),
		m_TextureType( GL_INVALID_ENUM ),
		m_Hash( FNV32_OFFSET )
    {
    }

    Texture::~Texture( )
    {
		this->Destroy( );
    }

    AUR_UINT32 Texture::LoadFromFile( const std::string &p_FileName )
    {
		if( p_FileName.size( ) == 0 )
		{
			return AUR_FAIL;
		}

		FILE *pFile = fopen( p_FileName.c_str( ), "r" );

		if( pFile == AUR_NULL )
		{
			std::cout << "[Aura::Texture::LoadFromFile] <ERROR> "
				"Failed to open texture: \"" << p_FileName << "\"" <<
			std::endl;

			return AUR_FAIL;		
		}

		TARGA_HEADER TargaHeader;

		fread( &TargaHeader, sizeof( TargaHeader ), 1, pFile );

		fseek( pFile, TargaHeader.IDLength, SEEK_CUR );

		AUR_MEMSIZE ExpectedImageSize = TargaHeader.Width *
			TargaHeader.Height * ( TargaHeader.BitsPerPixel / 8 );

		AUR_BYTE *pImageData = new AUR_BYTE[ ExpectedImageSize ];

		fread( pImageData, ExpectedImageSize, 1, pFile );

		fclose( pFile );
		pFile = AUR_NULL;

		GLenum Format, Type;

		switch( TargaHeader.BitsPerPixel )
		{
			case 16:
			{
				Format = GL_RGBA;
				Type = GL_UNSIGNED_SHORT_5_5_5_1;
				break;
			}
			case 24:
			{
				Format = GL_RGB;
				Type = GL_UNSIGNED_BYTE;
				break;
			}
			case 32:
			{
				Format = GL_RGBA;
				Type = GL_UNSIGNED_BYTE;
				break;
			}
			default:
			{
				std::cout << "[Aura::Texture::LoadFromFile] <ERROR> "
					"Unexpected bits per pixel, got: " <<
					TargaHeader.BitsPerPixel << std::endl;

				SafeDeleteArray< AUR_BYTE >( pImageData );

				return AUR_FAIL;
			}
		}

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		glGenTextures( 1, &m_TextureID );
		GLenum Error;
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		glTexImage2D( GL_TEXTURE_2D, 0, Format, TargaHeader.Width,
			TargaHeader.Height, 0, Format, Type, pImageData );

		m_Hash = HashBufferFNV1a( pImageData, TargaHeader.Width *
			TargaHeader.Height * ( TargaHeader.BitsPerPixel / 8 ) );

		SafeDeleteArray< AUR_BYTE >( pImageData );

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		m_Width = TargaHeader.Width;
		m_Height = TargaHeader.Height;

		return AUR_OK;
    }

    void Texture::Destroy( )
    {
    }

	AUR_UINT32 Texture::Activate( )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		return AUR_OK;
	}

	AUR_UINT16 Texture::GetWidth( ) const
	{
		return m_Width;
	}

	AUR_UINT16 Texture::GetHeight( ) const
	{
		return m_Height;
	}

	AUR_UINT32 Texture::GetHash( ) const
	{
		return m_Hash;
	}
}

