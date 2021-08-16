in vec3 inPosition;

void main( void ){
	gl_Position = vec4( inPosition.xy * vec2( 2 ) - vec2( 1 ), 1, 1 );
}
