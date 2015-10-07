#ifndef __AURA_MODEL_HPP__
#define __AURA_MODEL_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>
#include <string>
#include <vector>

namespace Aura
{
#pragma pack( 1 )
	struct MODEL_HEADER
	{
		AUR_SCHAR	ID[ 4 ];
		AUR_SCHAR	Name[ 32 ];
	};

	struct MODEL_MESH
	{
		AUR_UINT32	Flags;
		AUR_UINT32	VertexCount;
		AUR_UINT32	MaterialHash;
		AUR_UINT16	TriangleListCount;
		AUR_UINT16	TriangleStripCount;
		AUR_UINT16	TriangleFanCount;
	};
#pragma pack( )

	const AUR_UINT16 CHUNK_MESH				= 0x0001;
	const AUR_UINT16 CHUNK_MATERIAL			= 0x0002;
	const AUR_UINT16 CHUNK_JOINT			= 0x0004;
	const AUR_UINT16 CHUNK_TRIANGLE_LIST	= 0x0008;
	const AUR_UINT16 CHUNK_TRIANGLE_STRIP	= 0x0010;
	const AUR_UINT16 CHUNK_TRIANGLE_FAN		= 0x0020;

	const AUR_UINT32 MESH_NORMAL_TANGENT	= 0x00000001;
	const AUR_UINT32 MESH_NORMAL_LOCAL		= 0x00000002;

	class Mesh;
	class MaterialManager;
	class Camera;

	class Model
	{
	public:
		Model( MaterialManager *p_pMaterialManager );
		~Model( );

		AUR_UINT32 LoadFromFile( const std::string &p_FileName );
		AUR_UINT32 Render( Camera &p_Camera );

		void SetPosition( const Vector3 &p_Position );
		void SetScale( const Vector3 &p_Scale );
		void SetOrientation( const Vector3 &p_Orientation );

		void ToggleWireframe( );
		void ToggleNormals( );

	private:
		Model( const Model &p_Other );
		Model &operator=( const Model &p_Other );

		AUR_UINT32 ReadMeshData( FILE *p_pFile );

		Vector3					m_Position;
		Vector3					m_Scale;
		Vector3					m_Orientation;
		std::vector< Mesh * >	m_MeshArray;
		MaterialManager			*m_pMaterialManager;
		std::string				m_Name;
	};
}

#endif // __AURA_MODEL_HPP__

