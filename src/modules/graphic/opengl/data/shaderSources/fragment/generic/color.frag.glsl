#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec4 pColor;

layout(location=0) out vec4 outColor;

// WithDepth
// vvvvvvvvv
#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

uniform HIGHP sampler2DArray texDepth;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	VARYING_BIND(0) flat in int vLayer;
#else
	const int vLayer = 0;
#endif

#include "shared/defren/sample_depth.glsl"
// ^^^^^^^^^
////////////

void main(void){
	if(WithDepth){
		float depth = sampleDepth(texDepth, ivec3(gl_FragCoord.xy, vLayer));
		
		if(InverseDepth){
			if(gl_FragCoord.z < depth) discard;
			
		}else{
			if(gl_FragCoord.z > depth) discard;
		}
	}
	
	outColor = pColor;
}
