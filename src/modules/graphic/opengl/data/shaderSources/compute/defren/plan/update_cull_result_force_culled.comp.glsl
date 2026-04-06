#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/plan/world_element_constants.glsl"

UBOLAYOUT_BIND(0) readonly buffer UpdateIndex{
	uvec4 pUpdateIndex[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer ElementCullResult{
	uvec4 pElementCullResult[];
};

UNIFORM_BIND(0) uniform uint pIndexCount;

#include "shared/defren/plan/element_cull_result_set.glsl"


layout(local_size_x=64) in;


void main(void){
	if(gl_GlobalInvocationID.x >= pIndexCount){
		return;
	}
	
	uint s = gl_GlobalInvocationID.x;
	uint t = pUpdateIndex[s / uint(4)][s % uint(4)];
	
	setElementCullResult(t, uint(0));
}
