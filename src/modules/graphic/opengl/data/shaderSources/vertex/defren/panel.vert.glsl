#include "shared/preamble.glsl"

uniform mat4x3 pMatrixModel;
uniform mat4 pMatrixVP;
uniform mat4 pMatrixVP2;

layout(location=0) in vec3 inPosition;
layout(location=4) in vec2 inTexCoord;

#ifdef VS_RENDER_STEREO
	layout(location=5) in int inLayer;
#else
	const int inLayer = 0;
#endif

#ifdef GS_RENDER_STEREO
	out vec2 vGSTexCoord;
	#define vTexCoord vGSTexCoord
	
#else
	VARYING_BIND(0) out vec2 vTexCoord;
#endif


void main( void ){
	vec3 position = pMatrixModel * vec4( inPosition, 1 );
	
	#ifdef GS_RENDER_STEREO
		gl_Position = vec4( position, 1 );
	#else
		gl_Position = ( inLayer == 0 ? pMatrixVP : pMatrixVP2 ) * vec4( position, 1 );
	#endif
	
	vTexCoord = inTexCoord;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
	#endif
}
