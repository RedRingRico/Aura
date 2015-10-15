precision mediump float;

uniform sampler2D Sprite;
uniform vec4 Colour;

varying vec2 f_ST;

void main( )
{
	gl_FragColor = texture2D( Sprite, f_ST ) * Colour;
}

