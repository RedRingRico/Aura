#include <Game.hpp>
#include <GLES2/gl2.h>
#include <unistd.h>
#include <iostream>
#include <RendererPrimitive.hpp>
#include <Shader.hpp>
#include <Texture.hpp>

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

		const char VertexSource[ ] =
		{
			"attribute vec3 Position;\n"
			"attribute vec2 ST;\n"
			"varying vec2 f_ST;\n"
			"void main( )\n"
			"{\n"
			"	f_ST = ST;\n"
			"	gl_Position = vec4( Position, 1.0 );\n"
			"}\n"
		};
		const char FragmentSource[ ] =
		{
			"precision mediump float;\n"
			"uniform sampler2D Texture;\n"
			"varying vec2 f_ST;\n"
			"void main( )\n"
			"{\n"
			"	gl_FragColor = texture2D( Texture, f_ST );\n"
			"}\n"
		};


		const char WireframeVertexSource[ ] =
		{
			"attribute vec3 Position;\n"
			"void main( )\n"
			"{\n"
			"	gl_Position = vec4( Position, 1.0 );\n"
			"}\n"
		};
		const char WireframeFragmentSource[ ] =
		{
			"precision mediump float;\n"
			"void main( )\n"
			"{\n"
			"	gl_FragColor = vec4( 0.0, 1.0, 0.0, 1.0 );\n"
			"}\n"
		};

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
			{ 0.5f, 0.5f, 0.0f, 1.0f, 1.0f },
			{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
			{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
			{ -0.5f, 0.5f, 0.0f, 0.0f, 1.0f }
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

		Shader SquareShader;

		SquareShader.AddShaderSource( SHADER_TYPE_VERTEX, VertexSource );
		SquareShader.AddShaderSource( SHADER_TYPE_FRAGMENT, FragmentSource );

		Shader WireframeShader;

		WireframeShader.AddShaderSource( SHADER_TYPE_VERTEX,
			WireframeVertexSource );
		WireframeShader.AddShaderSource( SHADER_TYPE_FRAGMENT,
			WireframeFragmentSource );

		Texture Texture512;

		if( Texture512.LoadFromFile( "Test/Textures/512x512.tga" ) !=
			AUR_OK )
		{
			this->PlatformTerminate( );
			return AUR_FAIL;
		}

		int Shader0 = 0;
		while( Run )
		{
			m_Gamepad.GetState( &GamepadState );

			if( GamepadState.Buttons & GAMEPAD_BUTTON_START )
			{
				Run = AUR_FALSE;
			}

			m_Renderer.Clear( );
			SquareShader.Activate( );
			SquareShader.SetConstantData( "Texture", &Shader0 );
			Texture512.Activate( );
			Square.Render( );
			WireframeShader.Activate( );
			Square.RenderWireframe( );
			m_Renderer.SwapBuffers( );
		}

		this->PlatformTerminate( );

		return AUR_OK;
	}
}

