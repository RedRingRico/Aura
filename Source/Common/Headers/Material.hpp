#ifndef __AURA_MATERIAL_HPP__
#define __AURA_MATERIAL_HPP__

#include <DataTypes.hpp>
#include <string>
#include <vector>

namespace Aura
{
	class MaterialManager;

	class Material
	{
	public:
		explicit Material( MaterialManager *p_pMaterialManager );
		~Material( );

		AUR_UINT32 CreateFromFile( const std::string &p_FileName );

		AUR_UINT32 GetHash( ) const;

		AUR_UINT32 GetShaderHash( ) const;

		AUR_UINT32 GetTextureHashes(
			std::vector< AUR_UINT32 > &p_TextureHashes ) const;

		AUR_UINT32 GetTextureCount( ) const;

	private:
		Material( const Material &p_Other );
		Material &operator=( const Material &p_Other );

		MaterialManager				*m_pMaterialManager;
		AUR_UINT32					m_Hash;
		AUR_UINT32					m_ShaderHash;
		std::vector< AUR_UINT32 >	m_TextureHashes;
	};
}

#endif // __AURA_MATERIAL_HPP__

