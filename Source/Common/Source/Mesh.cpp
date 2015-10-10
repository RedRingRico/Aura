#include <Mesh.hpp>
#include <Camera.hpp>
#include <MaterialManager.hpp>
#include <RendererPrimitive.hpp>
#include <FNV.hpp>

namespace Aura
{
	Mesh::Mesh( MaterialManager *p_pMaterialManager ) :
		m_MaterialHash( FNV32_OFFSET ),
		m_DrawWireframe( AUR_FALSE ),
		m_DrawNormals( AUR_FALSE ),
		m_pMaterialManager( p_pMaterialManager ),
		m_SolidColourMaterial( FNV32_OFFSET )
	{
		// BAD!
		m_pMaterialManager->GetMaterialHash( "Solid Colour",
			m_SolidColourMaterial );
		
		m_WireframeColour[ 0 ] = m_WireframeColour[ 2 ] = 0.0f;
		m_WireframeColour[ 1 ] = m_WireframeColour[ 3 ] = 1.0f;
	}

	Mesh::~Mesh( )
	{
	}

	AUR_UINT32 Mesh::AddPrimitive( RendererPrimitive *p_pPrimitive )
	{
		m_Primitives.push_back( p_pPrimitive );

		return AUR_OK;
	}

	AUR_UINT32 Mesh::SetMaterial( const AUR_UINT32 p_MaterialHash )
	{
		m_MaterialHash = p_MaterialHash;

		return AUR_OK;
	}

	AUR_UINT32 Mesh::Render( const Camera &p_Camera )
	{
		AUR_FLOAT32 World[ 16 ], View[ 16 ], Projection[ 16 ];

		Matrix4x4 WorldMatrix, ViewMatrix, ProjectionMatrix;

		p_Camera.GetViewMatrix( ViewMatrix );
		p_Camera.GetProjectionMatrix( ProjectionMatrix );

		WorldMatrix = m_ScaleMatrix * m_RotationZ * m_RotationY * m_RotationX *
			m_Translation;

		WorldMatrix.AsArray( World );
		ViewMatrix.AsArray( View );
		ProjectionMatrix.AsArray( Projection );

		m_pMaterialManager->Activate( m_MaterialHash );

		m_pMaterialManager->SetConstantData( m_MaterialHash,
			"World", World );
		m_pMaterialManager->SetConstantData( m_MaterialHash,
			"View", View );
		m_pMaterialManager->SetConstantData( m_MaterialHash,
			"Projection", Projection );

		for( AUR_MEMSIZE Primitive = 0; Primitive < m_Primitives.size( );
			++Primitive )
		{
			m_Primitives[ Primitive ]->Render( );
		}

		if( m_DrawWireframe )
		{
			m_pMaterialManager->Activate( m_SolidColourMaterial );

			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"Colour", m_WireframeColour );
			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"World", World );
			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"View", View );
			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"Projection", Projection );

			glDisable( GL_DEPTH_TEST );
			for( AUR_MEMSIZE Primitive = 0; Primitive < m_Primitives.size( );
				++Primitive )
			{
				m_Primitives[ Primitive ]->RenderWireframe( );
			}
			glEnable( GL_DEPTH_TEST );
		}

		if( m_DrawNormals )
		{
			AUR_FLOAT32 NormalsColour[ 4 ] = { 0.0f, 0.5f, 1.0f, 1.0f };

			m_pMaterialManager->Activate( m_SolidColourMaterial );

			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"Colour", NormalsColour );
			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"World", World );
			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"View", View );
			m_pMaterialManager->SetConstantData( m_SolidColourMaterial,
				"Projection", Projection );

			for( AUR_MEMSIZE Primitive = 0; Primitive < m_Primitives.size( );
				++Primitive )
			{
				m_Primitives[ Primitive ]->RenderNormals( );
			}
		}

		return AUR_OK;
	}

	void Mesh::SetPosition( const Vector3 &p_Position )
	{
		m_Position = p_Position;
	}

	void Mesh::SetScale( const Vector3 &p_Scale )
	{
		m_Scale = p_Scale;
	}

	void Mesh::SetOrientation( const Vector3 &p_Orientation )
	{
		m_Orientation = p_Orientation;

		m_RotationY.RotateY( p_Orientation.GetY( ) );
	}

	void Mesh::ToggleWireframe( )
	{
		m_DrawWireframe = !m_DrawWireframe;
	}

	void Mesh::ToggleNormals( )
	{
		m_DrawNormals = !m_DrawNormals;
	}

	void Mesh::SetWireframeColour( const AUR_FLOAT32 p_Red,
			const AUR_FLOAT32 p_Green, const AUR_FLOAT32 p_Blue )
	{
		m_WireframeColour[ 0 ] = p_Red;
		m_WireframeColour[ 1 ] = p_Green;
		m_WireframeColour[ 2 ] = p_Blue;
	}
}

