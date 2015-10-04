#include <Game.hpp>
#include <GLES2/gl2.h>
#include <unistd.h>
#include <iostream>
#include <RendererPrimitive.hpp>
#include <Matrix4x4.hpp>
#include <VertexAttributes.hpp>
#include <MaterialManager.hpp>
#include <Camera.hpp>

namespace Aura
{
	Game::Game( )
	{
	}

	Game::~Game( )
	{
		m_Window.Destroy( );
	}

	AUR_UINT32 Game::Initialise( )
	{
		if( this->PlatformInitialise( ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Initialise] <ERROR> "
				"Failed to perform platform-specific initialisation" <<
				std::endl;

			return AUR_FAIL;
		}

		if( m_Window.Create( ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Initialise] <ERROR> "
				"Failed to create window" << std::endl;

			this->PlatformTerminate( );

			return AUR_FAIL;
		}

		if( m_Renderer.Create( m_Window ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Initialise] <ERROR> "
				"Failed to create renderer" << std::endl;

			this->PlatformTerminate( );

			return AUR_FAIL;
		}

		if( m_Gamepad.Initialise( ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Initialise] <ERROR> "
				"Failed to initialise the gamepad" << std::endl;

			this->PlatformTerminate( );

			return AUR_FAIL;
		}

		m_Renderer.SetClearFlags( AUR_TRUE, AUR_TRUE, AUR_TRUE );
		m_Renderer.SetClearColour( 0.0f, 17.0f / 255.0f, 43.0f / 255.0f );

		return AUR_OK;
	}

	AUR_UINT32 Game::Execute( )
	{
		GAMEPAD_STATE GamepadState;
		AUR_BOOL Run = AUR_TRUE;

		struct VERTEX
		{
			AUR_FLOAT32 X;
			AUR_FLOAT32 Y;
			AUR_FLOAT32 Z;
			AUR_FLOAT32 S;
			AUR_FLOAT32 T;
		};

		VERTEX Vertices[ ] =
		{
			{ 0.5f, 0.5f, -50.0f, 1.0f, 1.0f },
			{ 0.5f, -0.5f, -50.0f, 1.0f, 0.0f },
			{ -0.5f, -0.5f, -50.0f, 0.0f, 0.0f },
			{ -0.5f, 0.5f, -50.0f, 0.0f, 1.0f }
		};

		AUR_UINT16 Indices[ ] =
		{
			0, 1, 2,
			0, 2, 3
		};

		RendererPrimitive Square;

		VertexAttributes VertexAttribs( 16 );

		VertexAttribs.AddVertexAttribute( VERTEXATTRIBUTE_TYPE_FLOAT3,
			VERTEXATTRIBUTE_INTENT_POSITION );

		VertexAttribs.AddVertexAttribute( VERTEXATTRIBUTE_TYPE_FLOAT2,
			VERTEXATTRIBUTE_INTENT_TEXTURECOORDINATE );

		Square.Create( 4, 6, ( AUR_BYTE * )Vertices,
			Indices, VertexAttribs, PRIMITIVE_TYPE_TRIANGLE_LIST );

		AUR_UINT32 TestMaterial, SolidColourMaterial;

		MaterialManager MatMan;

		if( MatMan.CreateFromFile( "Test/Materials/Test.material",
			TestMaterial ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Execute] <ERROR> "
				"Failed to create Test material from file" << std::endl;

			return AUR_FAIL;
		}

		if( MatMan.CreateFromFile(
				"Test/Materials/PositionSolidColour.material",
				SolidColourMaterial ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Execute] <ERROR> "
				"Failed to create PositionSolidColour material from file" <<
				std::endl;

			return AUR_FAIL;
		}

		int Shader0 = 0;
		float Colour[ 4 ] = { 1.0f, 0.0f, 0.0f, 1.0f };

		Camera TestCamera;

		TestCamera.SetAspectRatio( 800.0f / 480.0f );
		TestCamera.SetProjectionMode( PROJECTIONMODE_PERSPECTIVE );
		TestCamera.SetClippingPlanes( 1.0f, 100000.0f );

		Matrix4x4 IdentityMatrix;
		Matrix4x4 ProjectionMatrix, ViewMatrix;

		float Identity[ 16 ];
		float View[ 16 ], Projection[ 16 ];

		IdentityMatrix.AsArray( Identity );

		TestCamera.CalculateProjectionMatrix( );

		TestCamera.GetProjectionMatrix( ProjectionMatrix );
		ProjectionMatrix.AsArray( Projection );

		TestCamera.CalculateViewMatrix( );
		TestCamera.GetViewMatrix( ViewMatrix );
		ViewMatrix.AsArray( View );

		while( Run )
		{
			m_Gamepad.GetState( &GamepadState );

			if( GamepadState.Buttons & GAMEPAD_BUTTON_START )
			{
				Run = AUR_FALSE;
			}

			TestCamera.CalculateViewMatrix( );
			TestCamera.GetViewMatrix( ViewMatrix );
			ViewMatrix.AsArray( View );

			m_Renderer.Clear( );
			MatMan.Activate( TestMaterial );
			MatMan.SetConstantData( TestMaterial, "Texture", &Shader0 );
			MatMan.SetConstantData( TestMaterial, "World", Identity );
			MatMan.SetConstantData( TestMaterial, "View", View );
			MatMan.SetConstantData( TestMaterial, "Projection",
				Projection );
			Square.Render( );
			MatMan.Activate( SolidColourMaterial );
			MatMan.SetConstantData( SolidColourMaterial, "Colour", Colour );
			MatMan.SetConstantData( SolidColourMaterial, "World", Identity );
			MatMan.SetConstantData( SolidColourMaterial, "View", View );
			MatMan.SetConstantData( SolidColourMaterial, "Projection",
				Projection );
			Square.RenderWireframe( );
			m_Renderer.SwapBuffers( );
		}

		this->PlatformTerminate( );

		return AUR_OK;
	}
}

