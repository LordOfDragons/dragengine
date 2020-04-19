#version 100

uniform mat3 pTransform;

attribute vec2 inPosition;

void main( void ){
	gl_Position = vec4( vec2( pTransform * vec3( inPosition, 1.0 ) ), 0.5, 1.0 );
}
