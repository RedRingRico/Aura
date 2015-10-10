precision mediump float;

uniform sampler2D Texture;
uniform vec3 EyePosition;
uniform float Shininess;

varying vec3 f_Position;
varying vec3 f_Normal;
varying vec2 f_ST;

void main( )
{
	vec3 LightPosition = vec3( 0.0, 50.0, 170.0 );
	vec3 LightColour = vec3( 1.0, 1.0, 1.0 );
	vec3 L = normalize( LightPosition - f_Position );
	float DiffuseLight = max( dot( f_Normal, L ), 0.0 );
	vec3 Diffuse = LightColour * DiffuseLight;
	vec3 V = normalize( EyePosition - f_Position );
	vec3 H = normalize( L + V );
	float SpecularLight = pow( max( dot( f_Normal, H ), 0.0 ), Shininess );
	vec3 Specular = vec3( 1.0, 1.0, 1.0 ) * LightColour * SpecularLight;
	
	gl_FragColor = texture2D( Texture, f_ST ) *
		vec4( Diffuse + Specular, 1.0 );
}

