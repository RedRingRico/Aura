attribute vec3 Position;
attribute vec2 ST;

uniform mat4 ProjectionViewWorld;

varying vec2 f_ST;

void main( )
{
	f_ST = ST;
	gl_Position = ProjectionViewWorld * vec4( Position, 1.0 );
}

