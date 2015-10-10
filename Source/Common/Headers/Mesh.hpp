#ifndef __AURA_MESH_HPP__
#define __AURA_MESH_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>
#include <Matrix4x4.hpp>
#include <vector>

namespace Aura
{
	class RendererPrimitive;
	class MaterialManager;
	class Camera;

	class Mesh
	{
	public:
		explicit Mesh( MaterialManager *p_pMaterialManager );
		~Mesh( );

		AUR_UINT32 AddPrimitive( RendererPrimitive *p_pPrimitive );

		AUR_UINT32 SetMaterial( const AUR_UINT32 p_MaterialHash );

		AUR_UINT32 Render( const Camera &p_Camera );

		void SetPosition( const Vector3 &p_Position );
		void SetScale( const Vector3 &p_Scale );
		void SetOrientation( const Vector3 &p_Orientation );

		void ToggleWireframe( );
		void ToggleNormals( );

		void SetWireframeColour( const AUR_FLOAT32 p_Red,
			const AUR_FLOAT32 p_Green, const AUR_FLOAT32 p_Blue );

	private:
		Mesh( const Mesh &p_Other );
		Mesh &operator=( const Mesh &p_Other );

		std::vector< RendererPrimitive * >	m_Primitives;

		Vector3	m_Position;
		Vector3	m_Scale;
		Vector3 m_Orientation;

		AUR_UINT32	m_MaterialHash;
		AUR_BOOL	m_DrawWireframe;
		AUR_BOOL	m_DrawNormals;

		MaterialManager	*m_pMaterialManager;

		AUR_UINT32	m_SolidColourMaterial;

		Matrix4x4	m_ScaleMatrix;
		Matrix4x4	m_RotationX;
		Matrix4x4	m_RotationY;
		Matrix4x4	m_RotationZ;
		Matrix4x4	m_Translation;

		AUR_FLOAT32	m_WireframeColour[ 4 ];
	};
}

#endif // __AURA_MESH_HPP__

