attribute vec3 Position;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 World;

void main( )
{
	gl_Position = Projection * View * World * vec4( Position, 1.0 );
}

