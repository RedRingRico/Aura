#include <Shader.hpp>
#include <FNV.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstring>

namespace Aura
{
	Shader::Shader( ) :
		m_ProgramID( 0 ),
		m_VertexShaderID( 0 ),
		m_FragmentShaderID( 0 ),
		m_Linked( AUR_FALSE ),
		m_Hash( FNV32_OFFSET )
	{
	}

	Shader::~Shader( )
	{
	}

	AUR_UINT32 Shader::AddShaderSource( const SHADER_TYPE p_Type,
		const char *p_pSource )
	{
		std::string ShaderTypeName;
		GLenum ShaderTypeGL;
		GLuint *pShader;

		switch( p_Type )
		{
			case SHADER_TYPE_VERTEX:
			{
				ShaderTypeName = "vertex";
				ShaderTypeGL = GL_VERTEX_SHADER;
				pShader = &m_VertexShaderID;

				break;
			}
			case SHADER_TYPE_FRAGMENT:
			{
				ShaderTypeName = "fragment";
				ShaderTypeGL = GL_FRAGMENT_SHADER;
				pShader = &m_FragmentShaderID;

				break;
			}
			case SHADER_TYPE_UNKNOWN:
			{
				std::cout << "[Aura::Shader::AddShaderSource] <ERROR> "
					"Unknown shader type" << std::endl;

				return AUR_FAIL;
			}
		}

		( *pShader ) = glCreateShader( ShaderTypeGL );
		glShaderSource( ( *pShader ), 1, &p_pSource, AUR_NULL );
		glCompileShader( ( *pShader ) );

		GLint Compile;
		glGetShaderiv( ( *pShader ), GL_COMPILE_STATUS, &Compile );

		if( Compile == GL_FALSE )
		{
			GLint LogLength;
			glGetShaderiv( ( *pShader ), GL_INFO_LOG_LENGTH, &LogLength );

			if( LogLength > 1 )
			{
				char *pLog = new char[ LogLength ];

				glGetShaderInfoLog( ( *pShader ), LogLength, AUR_NULL, pLog );
				std::cout << "[Aura::Shader::AddShaderSource] <ERROR> "
					"Failed to compile " << ShaderTypeName << " shader:" <<
					std::endl << pLog << std::endl;
				
				std::cout << "Shader source:" << std::endl;
				std::cout << p_pSource << std::endl;

				SafeDeleteArray< char >( pLog );
			}
		}

		if( p_Type == SHADER_TYPE_VERTEX )
		{
			if( this->ExtractAttributesFromSource( p_pSource ) != AUR_OK )
			{
				std::cout << "[Aura::Shader::AddShaderSource] <ERROR> "
					"Failed to extract the vertex attributes from the source "
					"code" << std::endl;
				return AUR_FAIL;
			}
		}

		m_Hash = HashStringFNV1a( p_pSource, m_Hash );

		return AUR_OK;
	}

	AUR_UINT32 Shader::SetConstantData( const AUR_MEMSIZE p_Index,
		void *p_pData )
	{
		if( m_Linked == AUR_FALSE )
		{
			if( this->Activate( ) == AUR_FAIL )
			{
				return AUR_FAIL;
			}
		}

		if( p_Index > m_ShaderConstants.size( ) )
		{
			return AUR_FAIL;
		}

		SHADER_CONSTANT Constant = m_ShaderConstants[ p_Index ];

		switch( Constant.Type )
		{
			case CONSTANT_TYPE_FLOAT1:
			{
				glUniform1fv( Constant.Location, Constant.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_FLOAT2:
			{
				glUniform2fv( Constant.Location, Constant.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_FLOAT3:
			{
				glUniform3fv( Constant.Location, Constant.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_FLOAT4:
			{
				glUniform4fv( Constant.Location, Constant.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT1:
			case CONSTANT_TYPE_SAMPLER_2D:
			case CONSTANT_TYPE_BOOL1:
			{
				glUniform1iv( Constant.Location, Constant.ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT2:
			case CONSTANT_TYPE_BOOL2:
			{
				glUniform2iv( Constant.Location, Constant.ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT3:
			case CONSTANT_TYPE_BOOL3:
			{
				glUniform3iv( Constant.Location, Constant.ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT4:
			case CONSTANT_TYPE_BOOL4:
			{
				glUniform4iv( Constant.Location, Constant.ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_MAT2X2:
			{
				glUniformMatrix2fv( Constant.Location, Constant.ArraySize,
					GL_FALSE, static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_MAT3X3:
			{
				glUniformMatrix3fv( Constant.Location, Constant.ArraySize,
					GL_FALSE, static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_MAT4X4:
			{
				glUniformMatrix4fv( Constant.Location, Constant.ArraySize,
					GL_FALSE, static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_UNKNOWN:
			{
				std::cout << "[Aura::Shader::SetConstantData] <ERROR> "
					"Unknown constant type" << std::endl;

				return AUR_FAIL;
			}
		}

		return AUR_OK;
	}

	AUR_UINT32 Shader::SetConstantData( const char *p_pName, void *p_pData )
	{
		if( m_Linked == AUR_FALSE )
		{
			if( this->Activate( ) == AUR_FAIL )
			{
				return AUR_FAIL;
			}
		}

		auto Constant = m_ShaderConstants.begin( );
		AUR_MEMSIZE StringLength = strlen( p_pName );

		while( Constant != m_ShaderConstants.end( ) )
		{
			if( ( *Constant ).Name.compare( 0, StringLength, p_pName ) == 0 )
			{
				break;
			}
			++Constant;
		}

		switch( ( *Constant ).Type )
		{
			case CONSTANT_TYPE_FLOAT1:
			{
				glUniform1fv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_FLOAT2:
			{
				glUniform2fv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_FLOAT3:
			{
				glUniform3fv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_FLOAT4:
			{
				glUniform4fv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT1:
			case CONSTANT_TYPE_SAMPLER_2D:
			case CONSTANT_TYPE_BOOL1:
			{
				glUniform1iv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT2:
			case CONSTANT_TYPE_BOOL2:
			{
				glUniform2iv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT3:
			case CONSTANT_TYPE_BOOL3:
			{
				glUniform3iv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_INT4:
			case CONSTANT_TYPE_BOOL4:
			{
				glUniform4iv( ( *Constant ).Location, ( *Constant ).ArraySize,
					static_cast< const GLint * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_MAT2X2:
			{
				glUniformMatrix2fv( ( *Constant ).Location,
					( *Constant ).ArraySize, GL_FALSE,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_MAT3X3:
			{
				glUniformMatrix3fv( ( *Constant ).Location,
				( *Constant ).ArraySize, GL_FALSE,
				static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_MAT4X4:
			{
				glUniformMatrix4fv( ( *Constant ).Location,
				( *Constant ).ArraySize, GL_FALSE,
				static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case CONSTANT_TYPE_UNKNOWN:
			{
				std::cout << "[Aura::Shader::Set( *Constant )Data] <ERROR> "
					"Unknown constant type" << std::endl;

				return AUR_FAIL;
			}
		}

		return AUR_OK;
	}

	AUR_UINT32 Shader::GetConstantIndex( const char *p_pName,
		AUR_MEMSIZE &p_Index )
	{
		if( m_Linked == AUR_FALSE )
		{
			if( this->Activate( ) != AUR_OK )
			{
				return AUR_FAIL;
			}
		}

		auto Constant = m_ShaderConstants.begin( );
		AUR_MEMSIZE StringLength = strlen( p_pName );

		while( Constant != m_ShaderConstants.end( ) )
		{
			if( ( *Constant ).Name.compare( 0, StringLength, p_pName ) == 0 )
			{
				break;
			}
			++Constant;
		}

		p_Index = ( *Constant ).Location;

		return AUR_OK;
	}
	
	AUR_MEMSIZE Shader::GetConstantCount( ) const
	{
		return m_ShaderConstants.size( );
	}

	AUR_UINT32 Shader::GetConstant( const AUR_MEMSIZE p_Index,
		SHADER_CONSTANT &p_Constant )
	{
		if( p_Index > m_ShaderConstants.size( ) )
		{
			return AUR_FAIL;
		}

		p_Constant = m_ShaderConstants[ p_Index ];

		return AUR_OK;
	}

	AUR_UINT32 Shader::Activate( )
	{
		if( m_Linked == AUR_FALSE )
		{
			if( this->Link( ) != AUR_OK )
			{
				return AUR_FAIL;
			}
			
			if( this->GetAttributes( ) != AUR_OK )
			{
				return AUR_FAIL;
			}

			if( this->GetConstants( ) != AUR_OK )
			{
				return AUR_FAIL;
			}

			m_Linked = AUR_TRUE;
		}

		glUseProgram( m_ProgramID );

		return AUR_OK;
	}

	AUR_UINT32 Shader::GetHash( ) const
	{
		return m_Hash;
	}

	AUR_UINT32 Shader::Link( )
	{
		if( glIsProgram( m_ProgramID ) )
		{
			glDeleteProgram( m_ProgramID );
		}

		m_ProgramID = glCreateProgram( );

		glAttachShader( m_ProgramID, m_VertexShaderID );
		glAttachShader( m_ProgramID, m_FragmentShaderID );

		glLinkProgram( m_ProgramID );

		auto Attribute = m_ShaderAttributesExplicit.begin( );

		while( Attribute != m_ShaderAttributesExplicit.end( ) )
		{
			glBindAttribLocation( m_ProgramID, ( *Attribute ).Location,
				( *Attribute ).Name.c_str( ) );
			++Attribute;
		}
		
		// Relink to apply the changes from glBindAttribLocation in
		// GetAttributes
		glLinkProgram( m_ProgramID );

		GLint Linked;
		glGetProgramiv( m_ProgramID, GL_LINK_STATUS, &Linked );

		if( Linked == GL_FALSE )
		{
			GLint LogLength;

			glGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH, &LogLength );

			if( LogLength > 1 )
			{
				char *pLog = new char[ LogLength ];

				glGetProgramInfoLog( m_ProgramID, LogLength, AUR_NULL, pLog );

				std::cout << "[Aura::Shader::Link] <ERROR> "
					"Failed to link shader: " << std::endl << pLog <<
					std::endl;

				SafeDeleteArray< char >( pLog );
			}

			glDeleteProgram( m_ProgramID );

			return AUR_FAIL;
		}

		glValidateProgram( m_ProgramID );

		GLint Validated;
		glGetProgramiv( m_ProgramID, GL_VALIDATE_STATUS, &Validated );

		if( Validated == GL_FALSE )
		{
			GLint LogLength;

			glGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH, &LogLength );

			if( LogLength > 1 )
			{
				char *pLog = new char[ LogLength ];

				glGetProgramInfoLog( m_ProgramID, LogLength, AUR_NULL, pLog );
				std::cout << "[Aura::Shader::Link] <ERROR> "
					"Shader did not pass validataion:" << std::endl << pLog <<
					std::endl;

				SafeDeleteArray< char >( pLog );
			}

			return AUR_FAIL;
		}

		return AUR_OK;
	}

	AUR_UINT32 Shader::GetConstants( )
	{
		GLint Constants, ConstantLength;
		glGetProgramiv( m_ProgramID, GL_ACTIVE_UNIFORMS, &Constants );
		glGetProgramiv( m_ProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH,
			&ConstantLength );

		m_ShaderConstants.clear( );

		for( GLint Constant = 0; Constant < Constants; ++Constant )
		{
			SHADER_CONSTANT ShaderConstant;
			GLenum ConstantType;
			char *pConstantName = new char[ ConstantLength ];

			glGetActiveUniform( m_ProgramID, Constant, ConstantLength,
				AUR_NULL, &ShaderConstant.ArraySize, &ConstantType,
				pConstantName );

			ShaderConstant.Name = pConstantName;

			SafeDeleteArray< char >( pConstantName );

			switch( ConstantType )
			{
				case GL_FLOAT:
				{
					ShaderConstant.Type = CONSTANT_TYPE_FLOAT1;
					break;
				}
				case GL_FLOAT_VEC2:
				{
					ShaderConstant.Type = CONSTANT_TYPE_FLOAT2;
					break;
				}
				case GL_FLOAT_VEC3:
				{
					ShaderConstant.Type = CONSTANT_TYPE_FLOAT3;
					break;
				}
				case GL_FLOAT_VEC4:
				{
					ShaderConstant.Type = CONSTANT_TYPE_FLOAT4;
					break;
				}
				case GL_INT:
				{
					ShaderConstant.Type = CONSTANT_TYPE_INT1;
					break;
				}
				case GL_INT_VEC2:
				{
					ShaderConstant.Type = CONSTANT_TYPE_INT2;
					break;
				}
				case GL_INT_VEC3:
				{
					ShaderConstant.Type = CONSTANT_TYPE_INT3;
					break;
				}
				case GL_INT_VEC4:
				{
					ShaderConstant.Type = CONSTANT_TYPE_INT4;
					break;
				}
				case GL_BOOL:
				{
					ShaderConstant.Type = CONSTANT_TYPE_BOOL1;
					break;
				}
				case GL_BOOL_VEC2:
				{
					ShaderConstant.Type = CONSTANT_TYPE_BOOL2;
					break;
				}
				case GL_BOOL_VEC3:
				{
					ShaderConstant.Type = CONSTANT_TYPE_BOOL3;
					break;
				}
				case GL_BOOL_VEC4:
				{
					ShaderConstant.Type = CONSTANT_TYPE_BOOL4;
					break;
				}
				case GL_FLOAT_MAT2:
				{
					ShaderConstant.Type = CONSTANT_TYPE_MAT2X2;
					break;
				}
				case GL_FLOAT_MAT3:
				{
					ShaderConstant.Type = CONSTANT_TYPE_MAT3X3;
					break;
				}
				case GL_FLOAT_MAT4:
				{
					ShaderConstant.Type = CONSTANT_TYPE_MAT4X4;
					break;
				}
				case GL_SAMPLER_2D:
				{
					ShaderConstant.Type = CONSTANT_TYPE_SAMPLER_2D;
					break;
				}
				case GL_SAMPLER_CUBE:
				{
					ShaderConstant.Type = CONSTANT_TYPE_SAMPLER_CUBE;
					break;
				}
			}

			ShaderConstant.Location = glGetUniformLocation( m_ProgramID,
				ShaderConstant.Name.c_str( ) );

			m_ShaderConstants.push_back( ShaderConstant );
		}

		return AUR_OK;
	}

	AUR_UINT32 Shader::GetAttributes( )
	{
		GLint Attributes, AttributeLength;

		glGetProgramiv( m_ProgramID, GL_ACTIVE_ATTRIBUTES, &Attributes );
		glGetProgramiv( m_ProgramID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
			&AttributeLength );

		m_ShaderAttributes.clear( );

		for( GLint Attribute = 0; Attribute < Attributes; ++Attribute )
		{
			SHADER_ATTRIBUTE ShaderAttribute;
			GLenum AttributeType;
			char *pAttributeName = new char[ AttributeLength ];

			glGetActiveAttrib( m_ProgramID, Attribute, AttributeLength,
				AUR_NULL, &ShaderAttribute.ArraySize, &AttributeType,
				pAttributeName );

			ShaderAttribute.Name = pAttributeName;
			ShaderAttribute.Location = glGetAttribLocation( m_ProgramID,
				pAttributeName );

			SafeDeleteArray< char >( pAttributeName );

			switch( AttributeType )
			{
				case GL_FLOAT:
				{
					ShaderAttribute.Type = VERTEXATTRIBUTE_TYPE_FLOAT1;
					break;
				}
				case GL_FLOAT_VEC2:
				{
					ShaderAttribute.Type = VERTEXATTRIBUTE_TYPE_FLOAT2;
					break;
				}
				case GL_FLOAT_VEC3:
				{
					ShaderAttribute.Type = VERTEXATTRIBUTE_TYPE_FLOAT3;
					break;
				}
				case GL_FLOAT_VEC4:
				{
					ShaderAttribute.Type = VERTEXATTRIBUTE_TYPE_FLOAT4;
					break;
				}
				case GL_FLOAT_MAT2:
				{
					ShaderAttribute.Type = VERTEXATTRIBUTE_TYPE_MAT2X2;
					break;
				}
				case GL_FLOAT_MAT3:
				{
					ShaderAttribute.Type = VERTEXATTRIBUTE_TYPE_MAT3X3;
					break;
				}
				case GL_FLOAT_MAT4:
				{
					ShaderAttribute.Type = VERTEXATTRIBUTE_TYPE_MAT4X4;
					break;
				}
			}

			m_ShaderAttributes.push_back( ShaderAttribute );
		}

		return AUR_OK;
	}

	AUR_UINT32 Shader::ExtractAttributesFromSource(
		const std::string &p_Shader )
	{
		AUR_MEMSIZE AttributePosition = p_Shader.find( "attribute" );

		GLint AttributeLocation = 0;

		m_ShaderAttributesExplicit.clear( );

		while( AttributePosition != std::string::npos )
		{
			AUR_MEMSIZE DelimiterPosition = p_Shader.find_first_of( ",;",
				AttributePosition );

			if( DelimiterPosition == std::string::npos )
			{
				std::cout << "[Aura::Shader::ExtractAttributesFromSource] "
					"<ERROR> Could not find a delimiter following the "
					"attribute" << std::endl;

				return AUR_FAIL;
			}

			// Skip the type
			AUR_MEMSIZE NameStart = p_Shader.find_first_of( " \t",
				AttributePosition );
			NameStart = p_Shader.find_first_not_of( " \t", NameStart );
			NameStart = p_Shader.find_first_of( " \t", NameStart );
			NameStart = p_Shader.find_first_not_of( " \t", NameStart );

			AUR_MEMSIZE NameEnd = p_Shader.find_first_of( " ,[;\t",
				NameStart );

			AUR_BOOL DoneProcessing = AUR_FALSE;

			do
			{
				std::string AttributeName = p_Shader.substr( NameStart,
					NameEnd - NameStart );

				SHADER_ATTRIBUTE Attribute;
				Attribute.Name = AttributeName;
				Attribute.Location = AttributeLocation;

				m_ShaderAttributesExplicit.push_back( Attribute );

				DoneProcessing = AUR_TRUE;
				
				++AttributeLocation;
			}while( DoneProcessing == AUR_FALSE );

			AttributePosition = p_Shader.find( "attribute",
				DelimiterPosition );
		}

		return AUR_OK;
	}
}

