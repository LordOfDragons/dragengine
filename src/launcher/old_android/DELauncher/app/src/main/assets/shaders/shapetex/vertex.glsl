#version 100

uniform mat3 pTransform;
uniform mat3 pTCTransform;

attribute vec2 inPosition;

varying vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( vec2( pTransform * vec3( inPosition, 1.0 ) ), 0.5, 1.0 );
	vTexCoord = vec2( pTCTransform * vec3( inPosition, 1.0 ) );
}
