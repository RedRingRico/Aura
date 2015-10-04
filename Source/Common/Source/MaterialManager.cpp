#include <MaterialManager.hpp>
#include <Material.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Memory.hpp>
#include <iostream>

namespace Aura
{
	MaterialManager::MaterialManager( )
	{
	}

	MaterialManager::~MaterialManager( )
	{
		this->Purge( );
	}

	AUR_UINT32 MaterialManager::CreateFromFile( const std::string &p_FileName,
		AUR_UINT32 &p_MaterialHash )
	{
		Material *pNewMaterial = new Material( this );

		if( pNewMaterial->CreateFromFile( p_FileName ) != AUR_OK )
		{
			std::cout << "[Aura::MaterialManager::CreateFromFile] <ERROR> "
				"Failed to create material: \"" << p_FileName << "\"" <<
				std::endl;

			return AUR_FAIL;
		}

		AUR_UINT32 MaterialHash = pNewMaterial->GetHash( );

		std::pair<
			std::map< AUR_UINT32, Material * >::iterator, bool > Insert;

		Insert = m_Materials.insert( std::pair< AUR_UINT32, Material * >(
			MaterialHash, pNewMaterial ) );

		p_MaterialHash = MaterialHash;

		if( Insert.second == false )
		{
			std::cout << "[Aura::Materialmanager::CreateFromFile] <INFO> "
				"Material \"" << p_FileName << "\" already exists" <<
				std::endl;

			SafeDelete< Material >( pNewMaterial );

			return AUR_OK;
		}

		return AUR_OK;
	}

	AUR_UINT32 MaterialManager::Activate( const AUR_UINT32 p_MaterialHash )
	{
		auto MaterialItr = m_Materials.find( p_MaterialHash );

		if( MaterialItr == m_Materials.end( ) )
		{
			std::cout << "[Aura::MaterialManager::Activate] <ERROR> "
				"Material hash: " << p_MaterialHash << " not found" <<
				std::endl;

			return AUR_FAIL;
		}

		AUR_UINT32 ShaderHash = MaterialItr->second->GetShaderHash( );

		auto ShaderItr = m_Shaders.find( ShaderHash );

		if( ShaderItr == m_Shaders.end( ) )
		{
			std::cout << "[Aura::MaterialManager::Activate] <ERROR> "
				"Failed to find the shader hash: " << ShaderHash <<
				std::endl;

			return AUR_FAIL;
		}

		if( MaterialItr->second->GetTextureCount( ) )
		{
			std::vector< AUR_UINT32 > TextureArray;
			MaterialItr->second->GetTextureHashes( TextureArray );

			for( AUR_MEMSIZE Texture = 0; Texture < TextureArray.size( );
				++Texture )
			{
				auto TextureItr = m_Textures.find( TextureArray[ Texture ] );

				if( TextureItr == m_Textures.end( ) )
				{
					std::cout << "[Aura::MaterialManager::Activate] <WARN> "
						"Texture hash " << TextureArray[ Texture ] <<
						" not valid" << std::endl;

					continue;
				}

				TextureItr->second->Activate( );
			}
		}

		return ShaderItr->second->Activate( );
	}

	AUR_UINT32 MaterialManager::SetConstantData(
		const AUR_UINT32 p_MaterialHash, const char *p_pConstantName,
		void *p_pData )
	{
		auto MaterialItr = m_Materials.find( p_MaterialHash );

		if( MaterialItr == m_Materials.end( ) )
		{
			std::cout << "[Aura::MaterialManager::SetConstantData] <ERROR> "
				"Could not find material hash: " << p_MaterialHash <<
				std::endl;

			return AUR_FAIL;
		}

		AUR_UINT32 ShaderHash = MaterialItr->second->GetShaderHash( );

		auto ShaderItr = m_Shaders.find( ShaderHash );

		if( ShaderItr == m_Shaders.end( ) )
		{
			std::cout << "[Aura::MaterialManager::SetConstantData] <ERROR> "
				"Could not find shader hash: " << ShaderHash << std::endl;

			return AUR_FAIL;
		}

		return ShaderItr->second->SetConstantData( p_pConstantName, p_pData );
	}

	AUR_UINT32 MaterialManager::Purge( )
	{
		for( auto Itr = m_Shaders.begin( ); Itr != m_Shaders.end( ); ++Itr )
		{
			SafeDelete< Shader >( Itr->second );
		}

		for( auto Itr = m_Textures.begin( ); Itr != m_Textures.end( ); ++Itr )
		{
			SafeDelete< Texture >( Itr->second );
		}

		for( auto Itr = m_Materials.begin( ); Itr != m_Materials.end( );
			++Itr )
		{
			SafeDelete< Material >( Itr->second );
		}

		m_Materials.clear( );
		m_Shaders.clear( );
		m_Textures.clear( );

		return AUR_OK;
	}

	AUR_UINT32 MaterialManager::RemoveMaterial(
		const AUR_UINT32 p_MaterialHash )
	{
		auto MaterialItr = m_Materials.find( p_MaterialHash );

		if( MaterialItr == m_Materials.end( ) )
		{
			std::cout << "[Aura::MaterialManager::RemoveMaterial] <ERROR> "
				"Could not find material hash: " << p_MaterialHash <<
				std::endl;

			return AUR_FAIL;
		}

		SafeDelete< Material >( MaterialItr->second );

		m_Materials.erase( MaterialItr );

		return AUR_OK;
	}

	AUR_UINT32 MaterialManager::CreateShader(
		const MATERIAL_SHADER &p_ShaderInfo, AUR_UINT32 &p_ShaderHash )
	{
		Shader *pNewShader = new Shader( );

		if( p_ShaderInfo.VertexSource.size( ) != 0 )
		{
			if( pNewShader->AddShaderSource( SHADER_TYPE_VERTEX,
				p_ShaderInfo.VertexSource.c_str( ) ) != AUR_OK )
			{
				std::cout << "[Aura::MaterialManager::CreateShader] <ERROR> "
					"Failed to create vertex shader" << std::endl;

				return AUR_FAIL;
			}
		}

		if( p_ShaderInfo.FragmentSource.size( ) != 0 )
		{
			if( pNewShader->AddShaderSource( SHADER_TYPE_FRAGMENT,
				p_ShaderInfo.FragmentSource.c_str( ) ) != AUR_OK )
			{
				std::cout << "[Aura::MaterialManager::CreateShader] <ERROR> "
					"Failed to create fragment shader" << std::endl;

				return AUR_FAIL;
			}
		}

		p_ShaderHash = pNewShader->GetHash( );

		std::pair< std::map< AUR_UINT32, Shader * >::iterator, bool > Insert;

		Insert = m_Shaders.insert( std::pair< AUR_UINT32, Shader * >(
			p_ShaderHash, pNewShader ) );

		if( Insert.second == false )
		{
			std::cout << "[Aura::MaterialManager::CreateShader] <WARN> "
				"Shader already exists" << std::endl;

			SafeDelete< Shader >( pNewShader );

			return AUR_OK;
		}

		return AUR_OK;
	}

	AUR_UINT32 MaterialManager::LoadTextureFromFile(
		const std::string &p_FileName, AUR_UINT32 &p_TextureHash )
	{
		Texture *pNewTexture = new Texture( );

		if( pNewTexture->LoadFromFile( p_FileName ) != AUR_OK )
		{
			std::cout << "[Aura::MaterialManager::LoadTextureFromFile] "
				"<ERROR> Failed to load texture: \"" << p_FileName << "\"" <<
				std::endl;

			return AUR_FAIL;
		}

		p_TextureHash = pNewTexture->GetHash( );

		std::pair< std::map< AUR_UINT32, Texture * >::iterator, bool > Insert;

		Insert = m_Textures.insert( std::pair< AUR_UINT32, Texture * >(
			p_TextureHash, pNewTexture ) );

		if( Insert.second == false )
		{
			std::cout << "[Aura::MaterialManager::LoadTextureFromFile] <WARN> "
				"Texture hash: " << p_TextureHash << " already exists" <<
				std::endl;

			SafeDelete< Texture >( pNewTexture );

			return AUR_OK;
		}

		return AUR_OK;
	}
}

