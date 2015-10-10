#include <Game.hpp>
#include <GLES2/gl2.h>
#include <unistd.h>
#include <iostream>
#include <RendererPrimitive.hpp>
#include <Matrix4x4.hpp>
#include <VertexAttributes.hpp>
#include <MaterialManager.hpp>
#include <Camera.hpp>
#include <Model.hpp>
#include <Arithmetic.hpp>
#include <cstring>

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

		Camera TestCamera;

		TestCamera.SetAspectRatio( 800.0f / 480.0f );
		TestCamera.SetProjectionMode( PROJECTIONMODE_PERSPECTIVE );
		TestCamera.SetClippingPlanes( 1.0f, 100000.0f );
		Vector3 CameraPosition( 0.0f, -350.0f, 500.0f );
		TestCamera.SetPosition( CameraPosition );
		TestCamera.SetLookPoint( Vector3( 0.0f, 100.0f, 0.0f ) );

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

		Model TheModel( &MatMan );

		Vector3 Rotate;

		if( TheModel.LoadFromFile( "Test/Models/WiggleBox.aura" ) != AUR_OK )
		{
			std::cout << "[Aura::Game::Execute] <ERROR> "
				"Failed to load model" << std::endl;

			this->PlatformTerminate( );

			return AUR_FAIL;
		}

		AUR_FLOAT32 Y = 0.0f;
		GAMEPAD_STATE OldGamepadState;

		AUR_FLOAT32 Shininess = 0.0f;

		while( Run )
		{
			m_Gamepad.GetState( &GamepadState );

			if( GamepadState.Buttons & GAMEPAD_BUTTON_START )
			{
				Run = AUR_FALSE;
			}

			if( ( ( GamepadState.Buttons & GAMEPAD_BUTTON_Y ) !=
					( OldGamepadState.Buttons & GAMEPAD_BUTTON_Y ) ) &&
				( GamepadState.Buttons & GAMEPAD_BUTTON_Y ) )
			{
				AUR_FLOAT32 Y = -CameraPosition.GetY( );
				CameraPosition.SetY( Y );
				TestCamera.SetPosition( CameraPosition );
			}

			if( ( ( GamepadState.Buttons & GAMEPAD_BUTTON_X ) !=
					( OldGamepadState.Buttons & GAMEPAD_BUTTON_X ) ) &&
				( GamepadState.Buttons & GAMEPAD_BUTTON_X ) )
			{
				TheModel.ToggleWireframe( );
			}

			if( ( ( GamepadState.Buttons & GAMEPAD_BUTTON_B ) !=
					( OldGamepadState.Buttons & GAMEPAD_BUTTON_B ) ) &&
				( GamepadState.Buttons & GAMEPAD_BUTTON_B ) )
			{
				TheModel.ToggleNormals( );
			}

			if( ( ( GamepadState.Buttons & GAMEPAD_BUTTON_A ) !=
					( OldGamepadState.Buttons & GAMEPAD_BUTTON_A ) ) &&
				( GamepadState.Buttons & GAMEPAD_BUTTON_A ) )
			{
				CameraPosition.Set( 0.0f, -350.0f, 500.0f );
			}

			if( Aura::Absolute( GamepadState.AnalogueStick[ 0 ].X ) >
				GAMEPAD_DEADZONE )
			{
				AUR_FLOAT32 X = CameraPosition.GetX( );
				X += GamepadState.AnalogueStick[ 0 ].X * 10.0f;

				CameraPosition.SetX( X );
			}

			if( Aura::Absolute( GamepadState.AnalogueStick[ 0 ].Y ) >
				GAMEPAD_DEADZONE )
			{
				AUR_FLOAT32 Y = CameraPosition.GetY( );
				Y += GamepadState.AnalogueStick[ 0 ].Y * 10.0f;

				CameraPosition.SetY( Y );
			}

			TestCamera.SetPosition( CameraPosition );

			TestCamera.CalculateViewMatrix( );
			TestCamera.GetViewMatrix( ViewMatrix );
			ViewMatrix.AsArray( View );

			m_Renderer.Clear( );
			MatMan.Activate( TestMaterial );
			MatMan.SetConstantData( TestMaterial, "EyePosition",
				&CameraPosition );
			MatMan.SetConstantData( TestMaterial, "Shininess", &Shininess );
			Y += 0.01f;
			Rotate.SetY( Y );
			TheModel.SetOrientation( Rotate );
			TheModel.Render( TestCamera );
			m_Renderer.SwapBuffers( );

			memcpy( &OldGamepadState, &GamepadState, sizeof( GamepadState ) );
		}

		this->PlatformTerminate( );

		return AUR_OK;
	}
}

