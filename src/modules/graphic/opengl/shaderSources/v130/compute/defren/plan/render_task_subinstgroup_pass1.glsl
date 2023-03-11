precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/render_task_sortable.glsl"


struct sCounter {
	uvec3 workGroupSize;
	uint counter;
};

UBOLAYOUT_BIND(0) readonly buffer Counters {
	sCounter pRenderTaskCounters;
};

UBOLAYOUT_BIND(1) readonly buffer RenderTask {
	sRenderTaskSortable pRenderTask[];
};

UBOLAYOUT_BIND(2) writeonly buffer SubInstGroup {
	uvec4 pSubInstGroup[];
};


layout( local_size_x=64 ) in;


layout( binding=0, offset=0 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=12 ) uniform atomic_uint pNextIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	uint stepCount = min( pRenderTaskCounters.counter, pRenderTask.length() );
	uint index = gl_GlobalInvocationID.x;
	if( index >= stepCount ){
		return;
	}
	
	// over all steps. determine starting point of sub instance groups. invocations
	// write their index to this SSBO if any of these conditions are true:
	// - first invocation
	// - subInstanceCount == 0
	// - any step[id-1].{pass,pipeline,tuc,vao,instance} not equal step[id]
	//   use atomic counter and also update workgroup size atomic counter
	bvec4 cond;
	
	cond.x = index > uint( 0 );
	cond.y = pRenderTask[ index ].params2.w > 0;
	
	uint prev = cond.x ? index - uint( 1 ) : uint( 0 );
	cond.z = all( equal( pRenderTask[ index ].params1, pRenderTask[ prev ].params1 ) );
	cond.w = pRenderTask[ index ].params2.x == pRenderTask[ prev ].params2.x;
	
	if( all( cond ) ){
		return;
	}
	
	uint nextIndex = atomicCounterIncrement( pNextIndex );
	
	pSubInstGroup[ nextIndex / uint( 4 ) ][ nextIndex % uint( 4 ) ] = index;
	
	if( nextIndex % dispatchWorkGroupSize == uint( 0 ) ){
		atomicCounterIncrement( pDispatchWorkGroupCount );
	}
}
