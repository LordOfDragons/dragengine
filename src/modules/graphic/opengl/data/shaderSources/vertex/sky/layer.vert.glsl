#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

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
	VARYING_BIND(0) out vec2 vTexCoord;
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

// Z value on far clip plane (cleared depth) in NDC
const float vZFar = InverseDepth ? -1.0 : 1.0;

void main( void ){
	gl_Position = vec4( inPosition, vZFar, 1 );
	vTexCoord = inPosition;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
