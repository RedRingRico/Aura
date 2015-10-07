attribute vec3 Position;
attribute vec3 Normal;
attribute vec2 ST;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;

varying vec3 f_Position;
varying vec3 f_Normal;
varying vec2 f_ST;

void main( )
{
	f_ST = ST;
	f_Normal = Normal;
	f_Position = Position;
	gl_Position = Projection * View * World * vec4( Position, 1.0 );
}

