#include <VertexAttributes.hpp>
#include <ReverseRange.hpp>
#include <cstring>
#include <iostream>

namespace Aura
{
	VertexAttributes::VertexAttributes(
		AUR_MEMSIZE p_MaximumVertexAttributes ) :
		m_MaximumVertexAttributes( p_MaximumVertexAttributes ),
		m_VertexAttributeCount( 0 ),
		m_VertexStride( 0 )
	{
	}

	VertexAttributes::~VertexAttributes( )
	{
	}

	AUR_UINT32 VertexAttributes::AddVertexAttribute(
		VERTEXATTRIBUTE_TYPE p_Type, VERTEXATTRIBUTE_INTENT p_Intent )
	{
		if( m_VertexAttributeCount > m_MaximumVertexAttributes )
		{
			std::cout << "[Aura::VertexAttributes::AddVertexAttribute] "
				"<ERROR> Vertex attribute capacity at maximum" << std::endl;
				
			return AUR_FAIL;
		}

		VERTEXATTRIBUTE Attribute = { p_Type, p_Intent };
		
		m_VertexAttributes.push_back( Attribute );
		m_VertexStride += ConvertVertexAttributeToSize( Attribute );

		++m_VertexAttributeCount;

		return AUR_OK;
	}

	VERTEXATTRIBUTE VertexAttributes::GetAttributeAt(
		AUR_MEMSIZE p_Index ) const
	{
		if( p_Index < m_VertexAttributeCount )
		{
			return m_VertexAttributes[ p_Index ];
		}

		VERTEXATTRIBUTE Unknown = { VERTEXATTRIBUTE_TYPE_UNKNOWN,
			VERTEXATTRIBUTE_INTENT_UNKNOWN };

		return Unknown;
	}

	AUR_MEMSIZE VertexAttributes::GetVertexAttributeCount( ) const
	{
		return m_VertexAttributeCount;
	}

	AUR_UINT32 VertexAttributes::GetVertexAttributes(
		std::vector< VERTEXATTRIBUTE > &p_VertexAttributes ) const
	{
		p_VertexAttributes = m_VertexAttributes;

		return AUR_OK;
	}

	AUR_MEMSIZE VertexAttributes::GetVertexStride( ) const
	{
		return m_VertexStride;
	}

	AUR_MEMSIZE ConvertVertexAttributeToSize(
		const VERTEXATTRIBUTE &p_VertexAttribute )
	{
		switch( p_VertexAttribute.Type )
		{
			case VERTEXATTRIBUTE_TYPE_FLOAT1:
			{
				return sizeof( AUR_FLOAT32 );
			}
			case VERTEXATTRIBUTE_TYPE_FLOAT2:
			{
				return sizeof( AUR_FLOAT32 ) * 2;
			}
			case VERTEXATTRIBUTE_TYPE_FLOAT3:
			{
				return sizeof( AUR_FLOAT32 ) * 3;
			}
			case VERTEXATTRIBUTE_TYPE_FLOAT4:
			case VERTEXATTRIBUTE_TYPE_MAT2X2:
			{
				return sizeof( AUR_FLOAT32 ) * 4;
			}
			case VERTEXATTRIBUTE_TYPE_MAT3X3:
			{
				return sizeof( AUR_FLOAT32 ) * 9;
			}
			case VERTEXATTRIBUTE_TYPE_MAT4X4:
			{
				return sizeof( AUR_FLOAT32 ) * 16;
			}
			case VERTEXATTRIBUTE_TYPE_UNKNOWN:
			default:
			{
				return 0;
			}
		}
	}

	AUR_MEMSIZE ConvertVertexAttributeToElementCount(
		const VERTEXATTRIBUTE &p_VertexAttribute )
	{
		switch( p_VertexAttribute.Type )
		{
			case VERTEXATTRIBUTE_TYPE_FLOAT1:
			{
				return 1;
			}
			case VERTEXATTRIBUTE_TYPE_FLOAT2:
			{
				return 2;
			}
			case VERTEXATTRIBUTE_TYPE_FLOAT3:
			{
				return 3;
			}
			case VERTEXATTRIBUTE_TYPE_FLOAT4:
			case VERTEXATTRIBUTE_TYPE_MAT2X2:
			{
				return 4;
			}
			case VERTEXATTRIBUTE_TYPE_MAT3X3:
			{
				return 9;
			}
			case VERTEXATTRIBUTE_TYPE_MAT4X4:
			{
				return 16;
			}
			case VERTEXATTRIBUTE_TYPE_UNKNOWN:
			default:
			{
				return 0;
			}
		}
	}

	GLenum ConvertVertexAttributeToGLenum(
		const VERTEXATTRIBUTE &p_VertexAttribute )
	{
		switch( p_VertexAttribute.Type )
		{
			case VERTEXATTRIBUTE_TYPE_FLOAT1:
			case VERTEXATTRIBUTE_TYPE_FLOAT2:
			case VERTEXATTRIBUTE_TYPE_FLOAT3:
			case VERTEXATTRIBUTE_TYPE_FLOAT4:
			case VERTEXATTRIBUTE_TYPE_MAT2X2:
			case VERTEXATTRIBUTE_TYPE_MAT3X3:
			case VERTEXATTRIBUTE_TYPE_MAT4X4:
			{
				return GL_FLOAT;
			}
			case VERTEXATTRIBUTE_TYPE_UNKNOWN:
			default:
			{
				return GL_INVALID_ENUM;
			}
		}
	}
}

