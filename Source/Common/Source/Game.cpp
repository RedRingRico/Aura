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
#include <Font.hpp>
#include <CLOC.hpp>
#include <Timer.hpp>
#include <FreeTimer.hpp>
#include <GitVersion.hpp>
#include <cstring>
#include <inttypes.h>
#include <sstream>
#include <NetworkSocket.hpp>
#include <ChatNetworkMessage.hpp>

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

		NetworkSocket TestSocket( 1, 1 );

		if( TestSocket.Open( AUR_NULL, SOCKET_TYPE_DGRAM,
			SOCKET_PROTOCOL_UDP ) != AUR_OK )
		{
			std::cout << "Failed to open a socket" << std::endl;
		}
		else
		{
			if( TestSocket.Connect( "128.199.103.82",
				"32500" ) != AUR_OK )
			{
				std::cout << "Failed to connect" << std::endl;
			}
			else
			{
				ChatNetworkMessage ChatMessage;
				ChatMessage.SetChatMessage( 10, 100, "Hello, there!" );

				TestSocket.Send( &ChatMessage );
			}
		}

		return AUR_OK;
	}

	AUR_UINT32 Game::Execute( )
	{
		GAMEPAD_STATE GamepadState;
		AUR_BOOL Run = AUR_TRUE;

		AUR_UINT32 TestMaterial, SolidColourMaterial;

		MaterialManager MatMan;

		AUR_UINT32 SpriteMat;

		if( MatMan.CreateFromFile( "Test/Materials/Sprite.material",
			SpriteMat ) != AUR_OK )
		{
			return AUR_FAIL;
		}

		m_pTestFont = new Font( &MatMan );
		m_pTestFont->LoadGlyphsFromFile( "Test/Fonts/Orbitron_Bold_14.xml" );
		m_pTestFont->LoadTextureFromFile(
			"Test/Textures/Orbitron_Bold_14.tga" );

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

		Camera ScreenCamera;

		ScreenCamera.SetAspectRatio( 800.0f / 480.0f );
		ScreenCamera.SetDimensions( 800.0f, 480.0f );
		ScreenCamera.SetProjectionMode( PROJECTIONMODE_ORTHOGRAPHIC );
		ScreenCamera.SetClippingPlanes( 1.0f, 1000.0f );
		ScreenCamera.SetPosition( Vector3( 0.0f, 0.0f, 0.0f ) );
		ScreenCamera.SetLookPoint( Vector3( 0.0f, 0.0f, -1.0f ) );
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

		AUR_UINT32 Counter = 0UL;

		FreeTimer FrameClock;
		FrameClock.Start( );

		FreeTimer GameTimer;
		GameTimer.Start( );

		AUR_UINT32 FrameCount = 0UL;
		AUR_UINT32 FrameRate = 0UL;

		std::stringstream InfoString1;
		InfoString1 << "Aura [" << GIT_TAG_NAME << "]";

		std::stringstream InfoString2;
		InfoString2 << GIT_BUILD_VERSION;

		AUR_UINT32 DotCounter = 0;

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
			ScreenCamera.CalculateViewMatrix( );
			ScreenCamera.CalculateProjectionMatrix( );

			m_pTestFont->RenderString( ScreenCamera, 0.0f,
				m_pTestFont->GetLineHeight( ), "%u", FrameRate );

			m_pTestFont->RenderString( ScreenCamera, 0.0f,
				480.0f, "%d lines of code",
				CLOC_LINECOUNT );
			m_pTestFont->RenderString( ScreenCamera, 0.0f,
				480.0f - ( m_pTestFont->GetLineHeight( ) * 1.4f ),
				"Elapsed time: %" PRIu64, GameTimer.GetSeconds( ) );

			AUR_FLOAT32 InfoLength;

			m_pTestFont->MeasureString( InfoLength,
				InfoString1.str( ).c_str( ) );

			m_pTestFont->RenderString( ScreenCamera,
				400.0f - ( InfoLength / 2.0f ), 
				480.0f - m_pTestFont->GetLineHeight( ) * 1.5f,
				InfoString1.str( ).c_str( ) );

			m_pTestFont->MeasureString( InfoLength,
				InfoString2.str( ).c_str( ) );

			m_pTestFont->RenderString( ScreenCamera,
				400.0f - ( InfoLength / 2.0f ), 
				480.0f,
				InfoString2.str( ).c_str( ) );

			m_pTestFont->MeasureString( InfoLength,
				"Connecting..." );

			m_pTestFont->RenderString( ScreenCamera,
				800.0f - InfoLength, 
				480.0f - m_pTestFont->GetLineHeight( ),
				"Connecting..." );

			for( AUR_UINT32 DotDraw = 0; DotDraw < DotCounter; ++DotDraw )
			{
			}

			m_Renderer.SwapBuffers( );
			
			++FrameCount;

			if( FrameClock.GetSeconds( ) >= 1 )
			{
				FrameRate = FrameCount;
				FrameCount = 0UL;
				FrameClock.Stop( );
				FrameClock.Start( );
			}

			memcpy( &OldGamepadState, &GamepadState, sizeof( GamepadState ) );
			++Counter;
		}

		this->PlatformTerminate( );

		return AUR_OK;
	}
}

