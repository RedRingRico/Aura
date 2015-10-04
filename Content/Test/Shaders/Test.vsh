attribute vec3 Position;
attribute vec2 ST;
//attribute vec3 Normal;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;

varying vec3 f_Position;
varying vec3 f_Normal;
varying vec2 f_ST;

void main( )
{
	f_ST = ST;
	//f_Normal = Normal;
	//f_Position = Position;
	gl_Position = vec4( Position, 1.0 ) * World * View * Projection;
}

