#ifndef __AURA_SHADER_HPP__
#define __AURA_SHADER_HPP__

#include <DataTypes.hpp>
#include <VertexAttributes.hpp>
#include <GLES2/gl2.h>
#include <vector>
#include <string>

namespace Aura
{
	enum SHADER_TYPE
	{
		SHADER_TYPE_UNKNOWN = 0,
		SHADER_TYPE_VERTEX,
		SHADER_TYPE_FRAGMENT
	};

	enum CONSTANT_TYPE
	{
		CONSTANT_TYPE_UNKNOWN = 0,
		CONSTANT_TYPE_FLOAT1,
		CONSTANT_TYPE_FLOAT2,
		CONSTANT_TYPE_FLOAT3,
		CONSTANT_TYPE_FLOAT4,
		CONSTANT_TYPE_INT1,
		CONSTANT_TYPE_INT2,
		CONSTANT_TYPE_INT3,
		CONSTANT_TYPE_INT4,
		CONSTANT_TYPE_BOOL1,
		CONSTANT_TYPE_BOOL2,
		CONSTANT_TYPE_BOOL3,
		CONSTANT_TYPE_BOOL4,
		CONSTANT_TYPE_MAT2X2,
		CONSTANT_TYPE_MAT3X3,
		CONSTANT_TYPE_MAT4X4,
		CONSTANT_TYPE_SAMPLER_2D,
		CONSTANT_TYPE_SAMPLER_CUBE
	};

	struct SHADER_CONSTANT
	{
		std::string		Name;
		CONSTANT_TYPE	Type;
		GLint			ArraySize;
		GLuint			Location;
	};

	struct SHADER_ATTRIBUTE
	{
		std::string				Name;
		VERTEXATTRIBUTE_TYPE	Type;
		GLint					ArraySize;
		GLuint					Location;
	};

	class Shader
	{
	public:
		Shader( );
		~Shader( );

		AUR_UINT32 AddShaderSource( const SHADER_TYPE p_Type,
			const char *p_pSource );

		AUR_UINT32 SetConstantData( const AUR_MEMSIZE p_Index, void *p_pData );
		AUR_UINT32 SetConstantData( const char *p_pName, void *p_pData );

		AUR_UINT32 GetConstantIndex( const char *p_pName,
			AUR_MEMSIZE &p_Index );

		AUR_MEMSIZE GetConstantCount( ) const;
		AUR_UINT32 GetConstant( const AUR_MEMSIZE p_Index,
			SHADER_CONSTANT &p_Constant );

		AUR_UINT32 Activate( );

		AUR_UINT32 GetHash( ) const;

	private:
		AUR_UINT32 Link( );
		AUR_UINT32 GetConstants( );
		AUR_UINT32 GetAttributes( );
		AUR_UINT32 ExtractAttributesFromSource( const std::string &p_Shader );

		GLuint							m_ProgramID;
		GLuint							m_VertexShaderID;
		GLuint							m_FragmentShaderID;
		std::vector< SHADER_CONSTANT >	m_ShaderConstants;
		std::vector< SHADER_ATTRIBUTE >	m_ShaderAttributes;
		std::vector< SHADER_ATTRIBUTE >	m_ShaderAttributesExplicit;
		AUR_BOOL						m_Linked;
		AUR_UINT32						m_Hash;
	};
}

#endif // __AURA_SHADER_HPP__

