#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

uniform mat4x3 pMatrixBody;

layout(location=0) in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	layout(location=1) in int inLayer;
#else
	const int inLayer = 0;
#endif

#ifdef GS_RENDER_STEREO
	out vec2 vGSTexCoord;
	#define vTexCoord vGSTexCoord
#else
	out vec2 vTexCoord;
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

const vec2 tcScale = vec2( 0.5, -0.5 );
const vec2 tcOffset = vec2( 0.5, 0.5 );

void main( void ){
	// apply layer rotation and position
	gl_Position = vec4( pMatrixBody * vec4( inPosition, 1, 1 ), 1 );
	
	#ifndef GS_RENDER_STEREO
		gl_Position = pMatrixSkyBody[ inLayer ] * gl_Position;
		
		// make sure Z is exactly -1 after transformation for depth test to work correctly
		gl_Position.z = -gl_Position.w;
	#endif
	
	vTexCoord = inPosition * tcScale + tcOffset;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
