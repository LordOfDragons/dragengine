#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform mat4x3 pMatrixMV;
uniform mat4 pMatrixProj;
uniform vec4 pAttributes1; // size, rotation, emissivity, n/a
uniform vec3 pPosition;

in vec3 inPosition;
in vec2 inTexCoord;

VARYING_BIND(0) out vec2 vTexCoord;
#ifdef USE_CLIP_PLANE
out vec3 vClipCoord;
#endif

const vec2 tcOffset = vec2( -0.5 );

void main( void ){
	vec3 position = pMatrixMV * vec4( pPosition /*inPosition*/, 1.0 );
	
	position.xy += mat2( cos( pAttributes1.y ), -sin( pAttributes1.y ), sin( pAttributes1.y ), cos( pAttributes1.y ) )
		* ( ( inTexCoord + tcOffset ) * vec2( pAttributes1.x ) );
	gl_Position = pMatrixProj * vec4( position, 1.0 );
	
	vTexCoord = inTexCoord;
#ifdef USE_CLIP_PLANE
	vClipCoord = position;
#endif
}
