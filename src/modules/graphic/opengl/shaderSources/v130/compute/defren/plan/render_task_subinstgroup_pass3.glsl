precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/render_task.glsl"
#include "v130/shared/defren/plan/counter.glsl"


UBOLAYOUT_BIND(0) readonly buffer SubInstGroup {
	uvec4 pSubInstGroup[];
};

UBOLAYOUT_BIND(1) readonly buffer Counters {
	sCounter pRenderComputeCounter[ pRenderComputeCounterCount ];
};

UBOLAYOUT_BIND(2) readonly buffer CountersRenderTask {
	sCounter pRenderTaskCounters;
};

UBOLAYOUT_BIND(3) buffer RenderTask {
	sRenderTask pRenderTask[];
};


layout( local_size_x=64 ) in;


uint getSubInstGroup( in uint index ){
	return pSubInstGroup[ index / uint( 4 ) ][ index % uint( 4 ) ];
}

void main( void ){
	uint count = pRenderComputeCounter[ erccRenderTaskSubInstanceGroups ].counter;
	uint index = gl_GlobalInvocationID.x;
	if( index >= count ){
		return;
	}
	
	uint first = getSubInstGroup( index );
	
	// over all SSBO invocations from pass 1. for each invocation which has
	// step[ssbo[id].index].subInstanceCount > 0 update the original step
	// subInstanceCount with:
	// - if not last invocation: step[id+1].index - step[id].index
	// - if last invocation: stepCount - step[id].index
	if( pRenderTask[ first ].subInstanceCount == uint( 0 ) ){
		return;
	}
	
	uint last = index < count - uint( 1 )
		? getSubInstGroup( index + uint( 1 ) )
		: pRenderTaskCounters.counter;
	
	pRenderTask[ first ].subInstanceCount = last - first;
}
