#ifndef __AURA_MATERIALMANAGER_HPP__
#define __AURA_MATERIALMANAGER_HPP__

#include <DataTypes.hpp>
#include <map>
#include <string>

namespace Aura
{
	struct MATERIAL_SHADER
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader;
	class Texture;
	class Material;

	class MaterialManager
	{
	public:
		MaterialManager( );
		~MaterialManager( );

		AUR_UINT32 CreateFromFile( const std::string &p_FileName,
			AUR_UINT32 &p_MaterialHash );

		AUR_UINT32 Activate( const AUR_UINT32 p_MaterialHash );

		AUR_UINT32 SetConstantData( const AUR_UINT32 p_MaterialHash,
			const char *p_pConstantName, void *p_pData );

		AUR_UINT32 GetMaterialHash( const std::string &p_MaterialName,
			AUR_UINT32 &p_MaterialHash );

		AUR_UINT32 Purge( );

		AUR_UINT32 RemoveMaterial( const AUR_UINT32 p_MaterialHash );

		AUR_UINT32 CreateShader( const MATERIAL_SHADER &p_ShaderInfo,
			AUR_UINT32 &p_ShaderHash );

		AUR_UINT32 LoadTextureFromFile( const std::string &p_FileName,
			AUR_UINT32 &p_TextureHash );

	private:
		MaterialManager( const MaterialManager &p_Other );
		MaterialManager &operator=( const MaterialManager &p_Other );

		std::map< AUR_UINT32, Material * >	m_Materials;
		std::map< std::string, Material * >	m_MaterialNames;
		std::map< AUR_UINT32, Shader * >	m_Shaders;
		std::map< AUR_UINT32, Texture * >	m_Textures;
	};
}

#endif // __AURA_MATERIALMANAGER_HPP__

