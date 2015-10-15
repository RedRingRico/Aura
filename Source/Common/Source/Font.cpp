#include <Font.hpp>
#include <RendererPrimitive.hpp>
#include <VertexAttributes.hpp>
#include <Camera.hpp>
#include <MaterialManager.hpp>
#include <Matrix4x4.hpp>
#include <FNV.hpp>
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <rapidxml.hpp>
#include <cstring>
#include <cstdlib>

namespace Aura
{
	Font::Font( MaterialManager *p_pMaterialManager ) :
		m_Hash( FNV32_OFFSET ),
		m_MaterialHash( FNV32_OFFSET ),
		m_pMaterialManager( p_pMaterialManager ),
		m_LineHeight( 0.0f )
	{
		// BAD!
		m_pMaterialManager->GetMaterialHash( "Sprite", m_MaterialHash );
		m_Colour[ 0 ] = m_Colour[ 1 ] = m_Colour[ 2 ] = m_Colour[ 3 ] = 1.0f;
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

	AUR_UINT32 Font::LoadGlyphsFromFile( const std::string &p_FileName )
	{
		FILE *pFile = fopen( p_FileName.c_str( ), "r" );

		if( pFile == AUR_NULL )
		{
			return AUR_FAIL;
		}

		fseek( pFile, 0L, SEEK_END );
		AUR_MEMSIZE FileSize = ftell( pFile );
		rewind( pFile );

		char *pSource = new char[ FileSize + 1];
		pSource[ FileSize ] = '\0';

		fread( pSource, 1, FileSize, pFile );

		rapidxml::xml_document< > Document;
		Document.parse< 0 >( pSource );

		rapidxml::xml_node< > *pNode = Document.first_node( );

		for( ; pNode; pNode = pNode->next_sibling( ) )
		{
			if( strcmp( pNode->name( ), "font" ) == 0 )
			{
				break;
			}
		}

		if( pNode == AUR_NULL )
		{
			std::cout << "[Aura::Font::LoadGlyphsFromFile] <ERROR> "
				"Could not find \"font\" element" << std::endl;

			return AUR_FAIL;
		}

		pNode = pNode->first_node( );

		AUR_FLOAT32 TextureWidth = 0, TextureHeight = 0;

		for( ; pNode; pNode = pNode->next_sibling( ) )
		{
			std::cout << "Node: " <<  pNode->name( ) << std::endl;

			// Get the texture information
			if( strcmp( pNode->name( ), "texture" ) == 0 )
			{
				TextureWidth =
					atof( pNode->first_attribute( "width" )->value( ) );
				TextureHeight =
					atof( pNode->first_attribute( "height" )->value( ) );
			}

			// Get the metrics
			if( strcmp( pNode->name( ), "metrics" ) == 0 )
			{
				m_LineHeight =
					atof( pNode->first_attribute( "height" )->value( ) );
			}

			// Extract all the chars
			if( strcmp( pNode->name( ), "chars" ) == 0 )
			{
				rapidxml::xml_node< > *pCharNode = pNode->first_node( );

				for( ; pCharNode; pCharNode = pCharNode->next_sibling( ) )
				{
					if( strcmp( pCharNode->name( ), "char" ) == 0 )
					{
						GLYPH Glyph;

						std::string StringBuffer;

						StringBuffer =
							pCharNode->first_attribute( "rect" )->value( );

						AUR_FLOAT32 X, Y, Width, Height;
						AUR_MEMSIZE StartPosition = 0;
						AUR_MEMSIZE EndPosition =  0;
						std::string Rect[ 4 ];
						AUR_MEMSIZE Count = 0;

						while( EndPosition != std::string::npos )
						{
							EndPosition = StringBuffer.find_first_of( ' ',
								EndPosition + 1 );

							Rect[ Count ] = StringBuffer.substr(
								StartPosition, EndPosition - StartPosition );

							StartPosition = EndPosition;
							++Count;
						}

						X = atof( Rect[ 0 ].c_str( ) );
						Y = atof( Rect[ 1 ].c_str( ) );
						Width = atof( Rect[ 2 ].c_str( ) );
						Height = atof( Rect[ 3 ].c_str( ) );

						// Polygons stored in terms of pixels
						// Top-right
						Glyph.Polygon[ 0 ].Position[ 0 ] = Width;
						Glyph.Polygon[ 0 ].Position[ 1 ] = Height;
						Glyph.Polygon[ 0 ].Position[ 2 ] = -1.0f;
						Glyph.Polygon[ 0 ].ST[ 0 ] = ( X + Width ) /
							TextureWidth;
						Glyph.Polygon[ 0 ].ST[ 1 ] = Y / TextureHeight;
						// Top-left
						Glyph.Polygon[ 1 ].Position[ 0 ] = 0.0f;
						Glyph.Polygon[ 1 ].Position[ 1 ] = Height;
						Glyph.Polygon[ 1 ].Position[ 2 ] = -1.0f;
						Glyph.Polygon[ 1 ].ST[ 0 ] = X / TextureWidth;
						Glyph.Polygon[ 1 ].ST[ 1 ] = Y / TextureHeight;
						// Bottom-left
						Glyph.Polygon[ 2 ].Position[ 0 ] = 0.0f;
						Glyph.Polygon[ 2 ].Position[ 1 ] = 0.0f;
						Glyph.Polygon[ 2 ].Position[ 2 ] = -1.0f;
						Glyph.Polygon[ 2 ].ST[ 0 ] = X / TextureWidth;
						Glyph.Polygon[ 2 ].ST[ 1 ] = ( Y + Height ) /
							TextureHeight;
						// Bottom-right
						Glyph.Polygon[ 3 ].Position[ 0 ] = Width;
						Glyph.Polygon[ 3 ].Position[ 1 ] = 0.0f;
						Glyph.Polygon[ 3 ].Position[ 2 ] = -1.0f;
						Glyph.Polygon[ 3 ].ST[ 0 ] = ( X + Width ) /
							TextureWidth;
						Glyph.Polygon[ 3 ].ST[ 1 ] = ( Y + Height ) /
							TextureHeight;

						Glyph.Width = ( AUR_UINT16 )( Width - X );
						Glyph.Height = ( AUR_UINT16 )( Height - Y );

						Glyph.XAdvance = atoi(
							pCharNode->first_attribute(
								"advance" )->value( ) );

						m_CharGlyphMap[ pCharNode->first_attribute(
							"id" )->value( )[ 0 ] ] = Glyph;
					}
				}
			}
		}

		return AUR_OK;
	}

	AUR_UINT32 Font::RenderString( const Camera &p_Camera, AUR_FLOAT32 p_X,
		AUR_FLOAT32 p_Y, const char *p_pFormat, ... )
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

		if( Return < 0 )
		{
			std::cout << "[Aura::Font::RenderString] <ERROR> "
				"Something went wrong creating the formatted string" <<
				std::endl;

			return AUR_FAIL;
		}

		CompleteString[ StringLength - 1 ] = '\0';

		RendererPrimitive StringPrimitives;

		AUR_FLOAT32 XPosition = 0.0f;

		AUR_MEMSIZE VertexCount, IndexCount;

		VertexCount = strlen( CompleteString ) * 4;
		IndexCount = strlen( CompleteString ) * 6;

		GLYPHVERTEX GlyphString[ VertexCount ];
		AUR_UINT16 StringIndices[ IndexCount ];

		for( AUR_MEMSIZE Index = 0; Index < IndexCount / 6; ++Index )
		{
			StringIndices[ Index * 6 ] = Index * 4;
			StringIndices[ ( Index * 6 ) + 1 ] = ( Index * 4 ) + 1;
			StringIndices[ ( Index * 6 ) + 2 ] = ( Index * 4 ) + 2;
			StringIndices[ ( Index * 6 ) + 3 ] = Index * 4;
			StringIndices[ ( Index * 6 ) + 4 ] = ( Index * 4 ) + 2;
			StringIndices[ ( Index * 6 ) + 5 ] = ( Index * 4 ) + 3 ;
		}

		for( AUR_MEMSIZE Char = 0; Char < StringLength; ++Char )
		{
			auto Glyph = m_CharGlyphMap.find( CompleteString[ Char ] );

			if( Glyph == m_CharGlyphMap.end( ) )
			{
				continue;
			}

			GLYPHVERTEX GlyphPolygon[ 4 ];
			memcpy( GlyphPolygon, Glyph->second.Polygon,
				sizeof( GlyphPolygon ) );

			GlyphPolygon[ 0 ].Position[ 0 ] += XPosition;
			GlyphPolygon[ 1 ].Position[ 0 ] += XPosition;
			GlyphPolygon[ 2 ].Position[ 0 ] += XPosition;
			GlyphPolygon[ 3 ].Position[ 0 ] += XPosition;

			memcpy( &GlyphString[ Char * 4 ], GlyphPolygon,
				sizeof( GlyphPolygon ) );

			XPosition += ( AUR_FLOAT32 )Glyph->second.XAdvance;
		}

		VertexAttributes VertexAttribs( 16 );
		VertexAttribs.AddVertexAttribute( VERTEXATTRIBUTE_TYPE_FLOAT3,
			VERTEXATTRIBUTE_INTENT_POSITION );
		VertexAttribs.AddVertexAttribute( VERTEXATTRIBUTE_TYPE_FLOAT2,
			VERTEXATTRIBUTE_INTENT_TEXTURECOORDINATE );

		RendererPrimitive StringPrimitive;
		StringPrimitive.Create( VertexCount, IndexCount,
			( AUR_BYTE * )GlyphString, StringIndices, VertexAttribs,
			PRIMITIVE_TYPE_TRIANGLE_LIST );

		AUR_FLOAT32 ProjectionViewWorldRaw[ 16 ];
		
		AUR_FLOAT32 ScreenWidth, ScreenHeight;
		p_Camera.GetDimensions( ScreenWidth, ScreenHeight );
		Matrix4x4 Projection, View, World;
		World( 0, 3 ) = -( ScreenWidth/ 2.0f ) + p_X;
		World( 1, 3 ) = ( ScreenHeight / 2.0f ) - p_Y;
		World( 2, 3 ) = 0.0f;
		p_Camera.GetProjectionMatrix( Projection );
		p_Camera.GetViewMatrix( View );
		Matrix4x4 ProjectionViewWorld = Projection * View * World;
		ProjectionViewWorld.AsArray( ProjectionViewWorldRaw );
		AUR_SINT32 Sprite = 0;

		m_pMaterialManager->Activate( m_MaterialHash );
		m_pMaterialManager->SetConstantData( m_MaterialHash,
			"ProjectionViewWorld", ProjectionViewWorldRaw );
		m_pMaterialManager->SetConstantData( m_MaterialHash, "Sprite",
			&Sprite );
		m_pMaterialManager->SetConstantData( m_MaterialHash, "Colour",
			m_Colour );
		m_Texture.Activate( );
		// There needs to be some kind of transparency stage for the renderer
		// to perform
		glEnable( GL_BLEND );
		glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
		glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
			GL_ZERO );
		StringPrimitive.Render( );
		glDisable( GL_BLEND );

		return AUR_OK;
	}

	AUR_FLOAT32 Font::GetLineHeight( ) const
	{
		return m_LineHeight;
	}
}

