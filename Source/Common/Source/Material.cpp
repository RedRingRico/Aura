#include <Material.hpp>
#include <MaterialManager.hpp>
#include <Shader.hpp>
#include <Memory.hpp>
#include <FNV.hpp>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace Aura
{
	Material::Material( MaterialManager *p_pMaterialManager ) :
		m_pMaterialManager( p_pMaterialManager ),
		m_Hash( FNV32_OFFSET ),
		m_ShaderHash( FNV32_OFFSET )
	{
	}

	Material::~Material( )
	{
	}

	AUR_UINT32 Material::CreateFromFile( const std::string &p_FileName )
	{
		if( p_FileName.size( ) == 0 )
		{
			std::cout << "[Aura::Material::CreateFromFile] <ERROR> "
				"File path is of zero length" << std::endl;

			return AUR_FAIL;
		}

		FILE *pFile = fopen( p_FileName.c_str( ), "r" );

		if( pFile == AUR_NULL )
		{
			std::cout << "[Aura::Material::CreateFromFile] <ERROR> "
				"Failed to open file: \"" << p_FileName << "\"" << std::endl;

			return AUR_FAIL;
		}

		fseek( pFile, 0L, SEEK_END );
		AUR_MEMSIZE FileSize = ftell( pFile );
		rewind( pFile );

		char *pSource = new char[ FileSize + 1 ];
		fread( pSource, 1, FileSize, pFile );

		pSource[ FileSize ] = '\0';

		fclose( pFile );
		pFile = AUR_NULL;

		rapidjson::Document MaterialJSON;
		MaterialJSON.Parse( pSource );

		AUR_UINT32 MaterialHash = HashStringFNV1a( pSource );

		SafeDeleteArray< char >( pSource );

		MATERIAL_SHADER MaterialShader;

		if( MaterialJSON.HasMember( "name" ) )
		{
			m_Name = MaterialJSON[ "name" ].GetString( );
		}
		else
		{
			std::cout << "[Aura::Material::CreateFromFile] <ERROR> "
				"No name available for material" << std::endl;

			return AUR_FAIL;
		}

		if( MaterialJSON.HasMember( "shader" ) )
		{
			rapidjson::Value &ShaderRoot = MaterialJSON[ "shader" ];

			if( ShaderRoot.HasMember( "source" ) )
			{
				if( ShaderRoot[ "source" ].IsArray( ) == false )
				{
					std::cout << "[Aura::Material::CreateFromFile] <ERROR> "
						"Failed to load shader source, it is not recognised "
						"as being an array of values" << std::endl;

					return AUR_FAIL;
				}

				rapidjson::Value &ShaderSourceRoot = ShaderRoot[ "source" ];

				for( rapidjson::SizeType i = 0; i < ShaderSourceRoot.Size( );
					++i )
				{
					std::string ShaderSource;
					SHADER_TYPE ShaderType = SHADER_TYPE_UNKNOWN;

					if( ShaderSourceRoot[ i ].HasMember( "type" ) )
					{
						std::string ShaderTypeString =
							ShaderSourceRoot[ i ][ "type" ].GetString( );

						if( ShaderTypeString.compare( "vertex" ) == 0 )
						{
							ShaderType = SHADER_TYPE_VERTEX;
						}
						else if( ShaderTypeString.compare( "fragment" ) == 0 )
						{
							ShaderType = SHADER_TYPE_FRAGMENT;
						}
						else
						{
							std::cout << "[Aura::Material::CreateFromFile] "
								"<ERROR> Unknown shader type: \"" <<
								ShaderTypeString << "\"" << std::endl;

							return AUR_FAIL;
						}
					}
					else
					{
						std::cout << "[Aura::Material::CreateFromFile] "
							"<ERROR> Could not find a \"type\" member" <<
							std::endl;

						return AUR_FAIL;
					}

					if( ShaderSourceRoot[ i ].HasMember( "path" ) )
					{
						FILE *pShaderFile = fopen(
							ShaderSourceRoot[ i ][ "path" ].GetString( ),
							"r" );

						if( pShaderFile == AUR_NULL )
						{
							std::cout << "[Aura::Material::CreateFromFile] "
								"<ERROR> Failed to open shader file: \"" <<
								ShaderSourceRoot[ i ][ "path" ].GetString( ) <<
								std::endl;

							return AUR_FAIL;
						}

						fseek( pShaderFile, 0L, SEEK_END );
						AUR_MEMSIZE ShaderLength = ftell( pShaderFile );
						rewind( pShaderFile );

						char *pShaderSource = new char[ ShaderLength + 1 ];
						pShaderSource[ ShaderLength ] = '\0';

						fread( pShaderSource, 1, ShaderLength, pShaderFile );

						fclose( pShaderFile );
						pShaderFile = AUR_NULL;

						ShaderSource.assign( pShaderSource );

						SafeDeleteArray< char >( pShaderSource );
					}
					else if( ShaderSourceRoot[ i ].HasMember( "code" ) )
					{
						ShaderSource =
							ShaderSourceRoot[ i ][ "code" ].GetString( );
					}
					else
					{
						std::cout << "[Aura::Material::CreateFromFile] "
							"<ERROR> Failed to find the shader path or code" <<
							std::endl;

						return AUR_FAIL;
					}

					switch( ShaderType )
					{
						case SHADER_TYPE_VERTEX:
						{
							MaterialShader.VertexSource = ShaderSource;
							break;
						}
						case SHADER_TYPE_FRAGMENT:
						{
							MaterialShader.FragmentSource = ShaderSource;
							break;
						}
						case SHADER_TYPE_UNKNOWN:
						default:
						{
							std::cout << "[Aura::Material::CreateFromFile] "
								"<ERROR> Unknown shader type" << std::endl;

							return AUR_FAIL;
						}
					}
				}
			}
			else
			{
				std::cout << "[Aura::Material::CreateFromFile] <ERROR> "
					"Failed to find a \"source\" member for the shader" <<
					std::endl;

				return AUR_FAIL;
			}
		}
		else
		{
			std::cout << "[Aura::Material::CreateFromFile] <ERROR> "
				"Failed to find a shader" << std::endl;

			return AUR_FAIL;
		}

		if( MaterialJSON.HasMember( "texture" ) )
		{
			rapidjson::Value &TextureRoot = MaterialJSON[ "texture" ];

			if( TextureRoot.IsArray( ) )
			{
				for( rapidjson::SizeType i = 0; i < TextureRoot.Size( ); ++i )
				{
					std::string TextureFile;

					if( TextureRoot[ i ].HasMember( "type" ) )
					{
						std::string TypeString =
							TextureRoot[ i ][ "type" ].GetString( );

						if( TypeString.compare( "albedo" ) == 0 )
						{
						}
						else
						{
							std::cout << "[Aura::Material::CreateFromFile] "
								"<WARN> Unknown texture type: \"" <<
								TypeString << "\"" << std::endl;
						}
					}
					else
					{
						std::cout << "[Aura::Material::CreateFromFile] "
							"<ERROR> No \"type\" member found for texture" <<
							std::endl;

						return AUR_FAIL;
					}

					if( TextureRoot[ i ].HasMember( "path" ) )
					{
						TextureFile = TextureRoot[ i ][ "path" ].GetString( );
					}
					else
					{
						std::cout << "[Aura::Material::CreateFromFile] " <<
							"<ERROR> No path for texture" << std::endl;

						return AUR_FAIL;
					}

					AUR_UINT32 TextureHash;

					if( m_pMaterialManager->LoadTextureFromFile( TextureFile,
						TextureHash ) != AUR_OK )
					{
						std::cout << "[Aura::Material::CreateFromFile] "
							"<ERROR> Could not load texture: \"" <<
							TextureFile << "\"" << std::endl;

						return AUR_FAIL;
					}

					m_TextureHashes.push_back( TextureHash );
				}
			}
			else
			{
				std::cout << "[Aura::Material::CreateFromFile] <ERROR> "
					"Textures are not in an array, aborting" << std::endl;

				return AUR_FAIL;
			}
		}

		m_pMaterialManager->CreateShader( MaterialShader, m_ShaderHash );

		m_Hash = MaterialHash;

		return AUR_OK;
	}

	AUR_UINT32 Material::GetHash( ) const
	{
		return m_Hash;
	}

	AUR_UINT32 Material::GetShaderHash( ) const
	{
		return m_ShaderHash;
	}

	AUR_UINT32 Material::GetTextureHashes(
		std::vector< AUR_UINT32 > &p_TextureHashes ) const
	{
		if( m_TextureHashes.size( ) == 0 )
		{
			std::cout << "[Aura::Material::GetTextureHashes] <ERROR> "
				"No textures present" << std::endl;

			return AUR_FAIL;
		}

		p_TextureHashes = m_TextureHashes;

		return AUR_OK;
	}

	AUR_UINT32 Material::GetTextureCount( ) const
	{
		return m_TextureHashes.size( );
	}

	AUR_UINT32 Material::GetName( std::string &p_Name ) const
	{
		p_Name = m_Name;

		return AUR_OK;
	}
}

