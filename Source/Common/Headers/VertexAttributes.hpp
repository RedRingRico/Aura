#ifndef __AURA_VERTEXATTRIBUTES_HPP__
#define __AURA_VERTEXATTRIBUTES_HPP__

#include <DataTypes.hpp>
#include <GLES2/gl2.h>
#include <vector>

namespace Aura
{
	enum VERTEXATTRIBUTE_TYPE
	{
		VERTEXATTRIBUTE_TYPE_UNKNOWN = 0,
		VERTEXATTRIBUTE_TYPE_FLOAT1,
		VERTEXATTRIBUTE_TYPE_FLOAT2,
		VERTEXATTRIBUTE_TYPE_FLOAT3,
		VERTEXATTRIBUTE_TYPE_FLOAT4,
		VERTEXATTRIBUTE_TYPE_MAT2X2,
		VERTEXATTRIBUTE_TYPE_MAT3X3,
		VERTEXATTRIBUTE_TYPE_MAT4X4
	};

	enum VERTEXATTRIBUTE_INTENT
	{
		VERTEXATTRIBUTE_INTENT_UNKNOWN = 0,
		VERTEXATTRIBUTE_INTENT_POSITION,
		VERTEXATTRIBUTE_INTENT_NORMAL,
		VERTEXATTRIBUTE_INTENT_TEXTURECOORDINATE,
		VERTEXATTRIBUTE_INTENT_COLOUR,
		VERTEXATTRIBUTE_INTENT_POINTSPRITE,
		VERTEXATTRIBUTE_INTENT_TANGENT,
		VERTEXATTRIBUTE_INTENT_BINORMAL
	};

	struct VERTEXATTRIBUTE
	{
		VERTEXATTRIBUTE_TYPE	Type;
		VERTEXATTRIBUTE_INTENT	Intent;
	};

	class VertexAttributes
	{
	public:
		explicit VertexAttributes( AUR_MEMSIZE p_MaximumVertexAttributes );
		~VertexAttributes( );

		AUR_UINT32 AddVertexAttribute( VERTEXATTRIBUTE_TYPE p_Type,
			VERTEXATTRIBUTE_INTENT p_Intent );

		VERTEXATTRIBUTE GetAttributeAt( AUR_MEMSIZE p_Index ) const;

		AUR_MEMSIZE GetVertexAttributeCount( ) const;
		AUR_UINT32 GetVertexAttributes(
			std::vector< VERTEXATTRIBUTE > &p_VertexAttributes ) const;

		AUR_MEMSIZE GetVertexStride( ) const;

	private:
		AUR_MEMSIZE						m_MaximumVertexAttributes;
		AUR_MEMSIZE						m_VertexAttributeCount;
		AUR_MEMSIZE						m_VertexStride;
		std::vector< VERTEXATTRIBUTE >	m_VertexAttributes;
	};

	AUR_MEMSIZE ConvertVertexAttributeToSize(
		const VERTEXATTRIBUTE &p_VertexAttribute );
	AUR_MEMSIZE ConvertVertexAttributeToElementCount(
		const VERTEXATTRIBUTE &p_VertexAttribute );
	GLenum ConvertVertexAttributeToGLenum(
		const VERTEXATTRIBUTE &p_VertexAttribute );
}

#endif // __AURA_VERTEXATTRIBUTES_HPP__

