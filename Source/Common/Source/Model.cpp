#include <Model.hpp>
#include <Chunk.hpp>
#include <Mesh.hpp>
#include <Camera.hpp>
#include <MaterialManager.hpp>
#include <VertexAttributes.hpp>
#include <RendererPrimitive.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstring>

namespace Aura
{
	Model::Model( MaterialManager *p_pMaterialManager ) :
		m_pMaterialManager( p_pMaterialManager )
	{
	}

	Model::~Model( )
	{
	}

	AUR_UINT32 Model::LoadFromFile( const std::string &p_FileName )
	{
		FILE *pFile = fopen( p_FileName.c_str( ), "r" );

		if( pFile == AUR_NULL )
		{
			std::cout << "[Aura::Model::LoadFromFile] <ERROR> "
				"File: \"" << p_FileName << "\" not found" << std::endl;

			return AUR_FAIL;
		}

		MODEL_HEADER ModelHeader;

		if( fread( &ModelHeader, sizeof( ModelHeader ), 1, pFile ) != 1 )
		{
			std::cout << "[Aura::Model::LoadFromFile] <ERROR> "
				"Could not load file header" << std::endl;

			fclose( pFile );
			pFile = AUR_NULL;

			return AUR_FAIL;
		}

		CHUNK Chunk;

		do
		{
			if( ReadChunk( Chunk, pFile ) != AUR_OK )
			{
				std::cout << "[Aura::Model::LoadFromFile] <ERROR> "
					"Could not load next chunk" << std::endl;

				fclose( pFile );
				pFile = AUR_NULL;

				return AUR_FAIL;
			}

			switch( Chunk.Type )
			{
				case CHUNK_MESH:
				{
					if( this->ReadMeshData( pFile ) != AUR_OK )
					{
						std::cout << "[Aura::Model::LoadFromFile] <ERROR> "
							"Failed to read mesh data" << std::endl;

						fclose( pFile );
						pFile = AUR_NULL;

						return AUR_FAIL;
					}
					break;
				}
				case CHUNK_END:
				{
					break;
				}
				default:
				{
					std::cout << "[Aura::Model::LoadFromFile] <ERROR> "
						"Unknown chunk type, reading past it" << std::endl;
					
					std::cout << "\tType: " << std::hex << Chunk.Type <<
						std::dec << std::endl;
					std::cout << "\tSize: " << Chunk.Size << std::endl;

					fseek( pFile, Chunk.Size, SEEK_CUR );

					if( ReadChunk( Chunk, pFile ) != AUR_OK )
					{
						std::cout << "[Aura::Model::LoadFromFile] <ERROR> "
							"Could not read next chunk" << std::endl;

						return AUR_FAIL;
					}

					if( ( Chunk.Type != CHUNK_END ) && ( Chunk.Size != 0 ) )
					{
						std::cout << "[Aura::Model::LoadFromFile] <ERROR> "
							"Unexpected chunk, was expecting end chunk" <<
							std::endl;

						return AUR_FAIL;
					}
				}
			}
		}while( Chunk.Type != CHUNK_END );

		if( pFile != AUR_NULL )
		{
			fclose( pFile );
			pFile = AUR_NULL;
		}

		return AUR_OK;
	}

	AUR_UINT32 Model::Render( Camera &p_Camera )
	{
		for( AUR_MEMSIZE Mesh = 0; Mesh < m_MeshArray.size( ); ++Mesh )
		{
			m_MeshArray[ Mesh ]->Render( p_Camera );
		}

		return AUR_OK;
	}

	void Model::SetOrientation( const Vector3 &p_Orientation )
	{
		for( AUR_MEMSIZE Mesh = 0; Mesh < m_MeshArray.size( ); ++Mesh )
		{
			m_MeshArray[ Mesh ]->SetOrientation( p_Orientation );
		}
	}

	void Model::ToggleWireframe( )
	{
		for( AUR_MEMSIZE Mesh = 0; Mesh < m_MeshArray.size( ); ++Mesh )
		{
			m_MeshArray[ Mesh ]->ToggleWireframe( );
		}
	}

	void Model::ToggleNormals( )
	{
		for( AUR_MEMSIZE Mesh = 0; Mesh < m_MeshArray.size( ); ++Mesh )
		{
			m_MeshArray[ Mesh ]->ToggleNormals( );
		}
	}

	AUR_UINT32 Model::ReadMeshData( FILE *p_pFile )
	{
		MODEL_MESH ModelMesh;

		fread( &ModelMesh, sizeof( ModelMesh ), 1, p_pFile );

		AUR_BYTE *pVertices = new AUR_BYTE[ 32 * ModelMesh.VertexCount ];

		VertexAttributes VertexAttribs( 8 );
		VertexAttribs.AddVertexAttribute( VERTEXATTRIBUTE_TYPE_FLOAT3,
			VERTEXATTRIBUTE_INTENT_POSITION );
		VertexAttribs.AddVertexAttribute( VERTEXATTRIBUTE_TYPE_FLOAT3,
			VERTEXATTRIBUTE_INTENT_NORMAL );
		VertexAttribs.AddVertexAttribute( VERTEXATTRIBUTE_TYPE_FLOAT2,
			VERTEXATTRIBUTE_INTENT_TEXTURECOORDINATE );

		fread( pVertices, VertexAttribs.GetVertexStride( ),
			ModelMesh.VertexCount, p_pFile );

		AUR_UINT16 *pListIndices =
			new AUR_UINT16[ ModelMesh.TriangleListCount * 3 ];

		fread( pListIndices, sizeof( AUR_UINT16 ), ModelMesh.TriangleListCount,
			p_pFile );

		Mesh *pNewMesh = new Mesh( m_pMaterialManager );

		RendererPrimitive *pListPrimitives = new RendererPrimitive( );

		pListPrimitives->Create( ModelMesh.VertexCount,
			ModelMesh.TriangleListCount, pVertices, pListIndices,
			VertexAttribs, PRIMITIVE_TYPE_TRIANGLE_LIST );

		pNewMesh->AddPrimitive( pListPrimitives );

		// For now, use the Test material
		AUR_UINT32 MaterialHash;
		m_pMaterialManager->GetMaterialHash( "Test", MaterialHash );

		pNewMesh->SetMaterial( MaterialHash );

		m_MeshArray.push_back( pNewMesh );

		SafeDeleteArray< AUR_UINT16 >( pListIndices );
		SafeDeleteArray< AUR_BYTE >( pVertices );

		CHUNK EndChunk;

		if( ReadChunk( EndChunk, p_pFile ) != AUR_OK )
		{
			std::cout << "[Aura::Model::ReadMeshData] <ERROR> "
				"Failed to load end chunk" << std::endl;

			return AUR_FAIL;
		}

		if( ( EndChunk.Type != CHUNK_END ) && ( EndChunk.Size != 0 ) )
		{
			std::cout << "[Aura::Model::ReadMeshData] <ERROR> "
				"Unexpected chunk, was expecting an end chunk" << std::endl;

			return AUR_FAIL;
		}

		return AUR_OK;
	}
}

