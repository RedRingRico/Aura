#include <RendererPrimitive.hpp>
#include <VertexAttributes.hpp>
#include <GLES2Extender.hpp>
#include <Vector3.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstring>

#define BUFFER_OFFSET( Offset ) ( ( unsigned char * ) AUR_NULL + ( Offset ) )

namespace Aura
{
	RendererPrimitive::RendererPrimitive( ) :
#if defined AURA_BUILD_DEBUG
		m_WireframeVertexBufferID( 0 ),
		m_WireframeIndexBufferID( 0 ),
		m_WireframeVertexArrayID( 0 ),
		m_NormalsVertexBufferID( 0 ),
		m_NormalsIndexBufferID( 0 ),
		m_NormalsVertexArrayID( 0 ),
#endif // AURA_BUILD_DEBUG
		m_PolygonCount( 0 ),
		m_VertexCount( 0 ),
		m_IndexCount( 0 ),
		m_VertexStride( 0 ),
		m_PrimitiveType( PRIMITIVE_TYPE_UNKNOWN ),
		m_VertexBufferID( 0 ),
		m_IndexBufferID( 0 ),
		m_VertexArrayID( 0 ),
		m_GLPrimitiveType( GL_INVALID_ENUM )
	{
	}

	RendererPrimitive::~RendererPrimitive( )
	{
		this->Destroy( );
	}

	AUR_UINT32 RendererPrimitive::Create( const AUR_MEMSIZE p_VertexCount,
		const AUR_MEMSIZE p_IndexCount, const AUR_BYTE *p_pVertices,
		const AUR_UINT16 *p_pIndices,
		const VertexAttributes &p_VertexAttributes,
		const PRIMITIVE_TYPE p_PrimitiveType )
	{
		if( p_pVertices == AUR_NULL )
		{
			std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
				"Pointer to vertex array is null" << std::endl;

			return AUR_FAIL;
		}

		if( p_pIndices == AUR_NULL )
		{
			std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
				"Pointer to index array is null" << std::endl;

			return AUR_FAIL;
		}

		switch( p_PrimitiveType )
		{
			case PRIMITIVE_TYPE_TRIANGLE_LIST:
			{
				m_PolygonCount = p_IndexCount / 3;
				m_GLPrimitiveType = GL_TRIANGLES;
				break;
			}
			case PRIMITIVE_TYPE_TRIANGLE_STRIP:
			{
				m_PolygonCount = p_IndexCount - 2;
				m_GLPrimitiveType = GL_TRIANGLE_STRIP;
				break;
			}
			case PRIMITIVE_TYPE_TRIANGLE_FAN:
			{
				m_PolygonCount = p_IndexCount - 2;
				m_GLPrimitiveType = GL_TRIANGLE_FAN;
				break;
			}
			case PRIMITIVE_TYPE_LINE_LIST:
			{
				m_PolygonCount = p_IndexCount / 2;
				m_GLPrimitiveType = GL_LINES;
				break;
			}
			case PRIMITIVE_TYPE_LINE_STRIP:
			{
				m_PolygonCount = p_IndexCount - 1;
				m_GLPrimitiveType = GL_LINE_STRIP;
				break;
			}
			case PRIMITIVE_TYPE_LINE_LOOP:
			{
				m_PolygonCount = p_IndexCount;
				m_GLPrimitiveType = GL_LINE_LOOP;
				break;
			}
			case PRIMITIVE_TYPE_UNKNOWN:
			{
				std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
					"Unknown primitive type" << std::endl;

				return AUR_FAIL;
			}
		}

		if( m_PolygonCount == 0 )
		{
			std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
				"Polygon count is zero" << std::endl;

			return AUR_FAIL;
		}

		m_PrimitiveType = p_PrimitiveType;

		m_VertexStride = p_VertexAttributes.GetVertexStride( );

		glGenBuffers( 1, &m_VertexBufferID );
		glGenBuffers( 1, &m_IndexBufferID );
		glGenVertexArrays( 1, &m_VertexArrayID );
		
		glBindVertexArray( m_VertexArrayID );
		glBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferID );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID );

		AUR_MEMSIZE Offset = 0;
		AUR_SINT32 PositionOffset = -1;
		AUR_SINT32 NormalsOffset = -1;

		for( AUR_MEMSIZE Index = 0;
			Index < p_VertexAttributes.GetVertexAttributeCount( ); ++Index )
		{
			AUR_MEMSIZE Dimension = 0;
			AUR_MEMSIZE TypeSize = 0;
			GLenum Type = GL_INVALID_ENUM;
			VERTEXATTRIBUTE VertexAttribute =
				p_VertexAttributes.GetAttributeAt( Index );

			Type = ConvertVertexAttributeToGLenum( VertexAttribute );
			TypeSize = ConvertVertexAttributeToSize( VertexAttribute );
			Dimension = ConvertVertexAttributeToElementCount(
				VertexAttribute );

			if( VertexAttribute.Intent == VERTEXATTRIBUTE_INTENT_POSITION )
			{
				PositionOffset = Offset;
			}

			if( VertexAttribute.Intent == VERTEXATTRIBUTE_INTENT_NORMAL )
			{
				NormalsOffset = Offset;
			}

			glVertexAttribPointer( Index, Dimension, Type, GL_FALSE,
				m_VertexStride, BUFFER_OFFSET( Offset ) );

			Offset += TypeSize;

			glEnableVertexAttribArray( Index );
		}

		glBufferData( GL_ARRAY_BUFFER, p_VertexCount * m_VertexStride,
			p_pVertices, GL_STATIC_DRAW );

		GLenum Error = glGetError( );

		if( Error == GL_OUT_OF_MEMORY )
		{
			this->Destroy( );

			std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
				"Failed to allocate vertices: No memory left on the device" <<
				std::endl;

			return AUR_FAIL;
		}

		glBufferData( GL_ELEMENT_ARRAY_BUFFER,
			p_IndexCount * sizeof( AUR_UINT16 ), p_pIndices, GL_STATIC_DRAW );

		Error = glGetError( );

		if( Error == GL_OUT_OF_MEMORY )
		{
			this->Destroy( );

			std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
				"Failed to allocate indices: No memory left on the device" <<
				std::endl;

			return AUR_FAIL;
		}

		m_VertexCount = p_VertexCount;
		m_IndexCount = p_IndexCount;

		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

#if defined AURA_BUILD_DEBUG
		// Build the wireframe
		if( PositionOffset != -1 )
		{
			glGenBuffers( 1, &m_WireframeVertexBufferID );
			glGenBuffers( 1, &m_WireframeIndexBufferID );
			glGenVertexArrays( 1, &m_WireframeVertexArrayID );

			glBindVertexArray( m_WireframeVertexArrayID );
			glBindBuffer( GL_ARRAY_BUFFER, m_WireframeVertexBufferID );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_WireframeIndexBufferID );

			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET( 0 ) );

			glEnableVertexAttribArray( 0 );

			struct POSITION
			{
				AUR_FLOAT32 Position[ 3 ];
			};
			POSITION *pPosition = new POSITION[ m_VertexCount ];

			for( AUR_MEMSIZE Vertex = 0; Vertex < m_VertexCount; ++Vertex )
			{
				memcpy( pPosition[ Vertex ].Position,
					p_pVertices + ( Vertex * m_VertexStride ) + PositionOffset,
					sizeof( POSITION ) );
			}

			glBufferData( GL_ARRAY_BUFFER, m_VertexCount * sizeof( POSITION ),
				pPosition, GL_STATIC_DRAW );

			Error = glGetError( );

			if( Error == GL_OUT_OF_MEMORY )
			{
				std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
					"Could not allocate wireframe vertices: No memory left on "
					"the device" << std::endl;

				this->Destroy( );

				return AUR_FAIL;
			}

			// Same order, different vertex data
			glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				m_IndexCount * sizeof( AUR_UINT16 ), p_pIndices,
				GL_STATIC_DRAW );

			Error = glGetError( );

			if( Error == GL_OUT_OF_MEMORY )
			{
				std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
					"Could not allocate wireframe indices: No memory left on "
					"the device" << std::endl;

				this->Destroy( );

				return AUR_FAIL;
			}

			glBindVertexArray( 0 );
			glBindBuffer( GL_ARRAY_BUFFER, 0 );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

			SafeDeleteArray< POSITION >( pPosition );
		}

		// Build the normals
		if( ( NormalsOffset != -1 ) && ( PositionOffset != 1 ) )
		{
			glGenBuffers( 1, &m_NormalsVertexBufferID );
			glGenBuffers( 1, &m_NormalsIndexBufferID );
			glGenVertexArrays( 1, &m_NormalsVertexArrayID );

			glBindVertexArray( m_NormalsVertexArrayID );
			glBindBuffer( GL_ARRAY_BUFFER, m_NormalsVertexBufferID );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_NormalsIndexBufferID );

			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET( 0 ) );

			glEnableVertexAttribArray( 0 );

			struct NORMALPOSITION
			{
				AUR_FLOAT32 Start[ 3 ];
				AUR_FLOAT32 End[ 3 ];
			};

			NORMALPOSITION *pNormalPosition =
				new NORMALPOSITION[ m_VertexCount ];

			for( AUR_MEMSIZE Vertex = 0; Vertex < m_VertexCount; ++Vertex )
			{
				memcpy( pNormalPosition[ Vertex ].Start,
					p_pVertices + ( Vertex * m_VertexStride ) + PositionOffset,
					12 );
				memcpy( pNormalPosition[ Vertex ].End,
					p_pVertices + ( Vertex * m_VertexStride ) + NormalsOffset,
					12 );

				Vector3 Normal( pNormalPosition[ Vertex ].End[ 0 ],
					pNormalPosition[ Vertex ].End[ 1 ],
					pNormalPosition[ Vertex ].End[ 2 ] );

				Vector3 Position( pNormalPosition[ Vertex ].Start[ 0 ],
					pNormalPosition[ Vertex ].Start[ 1 ],
					pNormalPosition[ Vertex ].Start[ 2 ] );

				Normal += Position;

				memcpy( pNormalPosition[ Vertex ].End, &Normal, 12 );
			}

			AUR_UINT16 *pNormalsIndices = new AUR_UINT16[ m_VertexCount * 2 ];

			for( AUR_MEMSIZE Index = 0; Index < m_VertexCount * 2; ++Index )
			{
				pNormalsIndices[ Index ] = Index;
			}

			glBufferData( GL_ARRAY_BUFFER,
				m_VertexCount * sizeof( NORMALPOSITION ), pNormalPosition,
				GL_STATIC_DRAW );

			Error = glGetError( );

			if( Error == GL_OUT_OF_MEMORY )
			{
				std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
					"Could not allocate normals vertices: No memory left on "
					"the device" << std::endl;

				this->Destroy( );

				return AUR_FAIL;
			}

			glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				sizeof( AUR_UINT16 ) * m_VertexCount * 2, pNormalsIndices,
				GL_STATIC_DRAW );

			Error = glGetError( );

			if( Error == GL_OUT_OF_MEMORY )
			{
				std::cout << "[Aura::RendererPrimitive::Create] <ERROR> "
					"Could not allocate normals indices: No memory left on "
					"the device" << std::endl;

				this->Destroy( );

				return AUR_FAIL;
			}

			glBindVertexArray( 0 );
			glBindBuffer( GL_ARRAY_BUFFER, 0 );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

			SafeDeleteArray< AUR_UINT16 >( pNormalsIndices );
			SafeDeleteArray< NORMALPOSITION >( pNormalPosition );
		}
#endif
		return AUR_OK;
	}

	void RendererPrimitive::Destroy( )
	{
#if defined AURA_BUILD_DEBUG
		glDeleteBuffers( 1, &m_WireframeVertexBufferID );
		glDeleteBuffers( 1, &m_WireframeIndexBufferID );
		glDeleteVertexArrays( 1, &m_WireframeVertexArrayID );

		glDeleteBuffers( 1, &m_NormalsVertexBufferID );
		glDeleteBuffers( 1, &m_NormalsIndexBufferID );
		glDeleteVertexArrays( 1, &m_NormalsVertexArrayID );
#endif
		glDeleteBuffers( 1, &m_VertexBufferID );
		glDeleteBuffers( 1, &m_IndexBufferID );
		glDeleteVertexArrays( 1, &m_VertexArrayID );
	}

	AUR_UINT32 RendererPrimitive::Render( )
	{
		if( m_PolygonCount == 0 )
		{
			std::cout << "[Aura::RendererPrimitive::Render] <ERROR> "
				"No primitives to render" << std::endl;

			return AUR_FAIL;
		}

		glBindVertexArray( m_VertexArrayID );

		glDrawElements( m_GLPrimitiveType, m_IndexCount, GL_UNSIGNED_SHORT,
			BUFFER_OFFSET( 0 ) );

		glBindVertexArray( 0 );

		return AUR_OK;
	}

	AUR_UINT32 RendererPrimitive::RenderWireframe( )
	{
#if defined AURA_BUILD_DEBUG
		if( m_PolygonCount == 0 )
		{
			std::cout << "[Aura::RendererPrimitive::RenderWireframe] <ERROR> "
				"No primitives to render" << std::endl;

			return AUR_FAIL;
		}

		glBindVertexArray( m_WireframeVertexArrayID );

		if( m_PrimitiveType == PRIMITIVE_TYPE_LINE_LIST )
		{
			for( AUR_MEMSIZE Index = 0; Index < m_IndexCount; Index += 2 )
			{
				glDrawElements( GL_LINES, 3, GL_UNSIGNED_SHORT,
					BUFFER_OFFSET( Index * sizeof( GLushort ) ) );
			}
		}
		else
		{
			for( AUR_MEMSIZE Index = 0; Index < m_IndexCount; Index += 3 )
			{
				glDrawElements( GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT,
					BUFFER_OFFSET( Index * sizeof( GLushort ) ) );
			}
		}

		glBindVertexArray( 0 );

#endif // AURA_BUILD_DEBUG
		return AUR_OK;
	}

	AUR_UINT32 RendererPrimitive::RenderNormals( )
	{
#if defined AURA_BUILD_DEBUG
		if( m_PolygonCount == 0 )
		{
			std::cout << "[Aura::RendererPrimitive::RenderNormals] <ERROR> "
				"No primitives to render" << std::endl;

			return AUR_FAIL;
		}

		glBindVertexArray( m_NormalsVertexArrayID );

		glDrawElements( GL_LINES, m_VertexCount * 2, GL_UNSIGNED_SHORT,
			BUFFER_OFFSET( 0 ) );

		glBindVertexArray( 0 );

#endif // AURA_BUIDL_DEBUG
		return AUR_OK;
	}
}

