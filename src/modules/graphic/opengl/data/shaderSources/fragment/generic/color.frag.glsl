#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pColor;

layout(location=0) out vec4 outColor;

// WithDepth
// vvvvvvvvv
#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

layout(binding=0) uniform HIGHP sampler2DArray texDepth;

#include "shared/defren/sample_depth.glsl"
// ^^^^^^^^^
////////////

#include "shared/interface/2d/fragment.glsl"

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
