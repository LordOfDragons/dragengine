precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"

uniform mat4x3 pMatrixBody;

in vec2 inPosition;

#ifdef GS_RENDER_STEREO
	out vec2 vGSTexCoord;
	#define vTexCoord vGSTexCoord
#else
	out vec2 vTexCoord;
#endif

const vec2 tcScale = vec2( 0.5, -0.5 );
const vec2 tcOffset = vec2( 0.5, 0.5 );

void main( void ){
	gl_Position = vec4( pMatrixBody * vec4( inPosition, 1, 1 ), 1 );
	#ifndef GS_RENDER_STEREO
		gl_Position = pMatrixSkyBody[ 0 ] * gl_Position;
	#endif
	
	vTexCoord = inPosition * tcScale + tcOffset;
}
