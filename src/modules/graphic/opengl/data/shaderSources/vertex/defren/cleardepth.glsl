uniform float pDepth;

in vec2 inPosition;

void main( void ){
	gl_Position = vec4( vec3( inPosition, pDepth ), 1.0 );
}
