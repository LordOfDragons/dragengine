precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/defines.glsl"
#include "v130/shared/defren/plan/intersect_frustum.glsl"
#include "v130/shared/defren/plan/intersect_gi.glsl"


UBOLAYOUT_BIND(3) writeonly buffer SearchNodes {
	uvec4 pSearchNodes[];
};


layout( local_size_x=64 ) in;

// the workgroup counts for the next indirect compute shader call is stored next
// to the total node counter like this:
// 
// uint ssbo[ 0 ] = dispatch.workgroup.x
// uint ssbo[ 1 ] = dispatch.workgroup.y
// uint ssbo[ 2 ] = dispatch.workgroup.z
// uint ssbo[ 3 ] = elementCount
// 
// we only need to update the workgroup.x value. the workgroup.y and workgroup.z
// are set by the opengl module to 1. the next node index is used to keep track
// of where to write the next node. glDispatchComputeIndirect does not care
// about the additional data so combining them in the same buffer is fine
layout( binding=4, offset=0) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=4, offset=12) uniform atomic_uint pNextNodeIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	// skip outside of parameter space
	if( gl_GlobalInvocationID.x >= pNodeCount ){
		return;
	}
	
	uint index = gl_GlobalInvocationID.x;
	vec3 minExtend = pWorldOctreeNode[ index ].minExtend;
	vec3 maxExtend = pWorldOctreeNode[ index ].maxExtend;
	
	// if node does not intersect the frustum nor the GI cascade box skip all content
	// inside. the GI cascade box test can be easily disabled by settings pGIMinExtend
	// equal to pGIMaxExtend preferably outside the frustum box
	if( ! notIntersectFrustum( minExtend, maxExtend )
	&& ! notIntersectGI( minExtend, maxExtend ) ){
		return;
	}
	
	// add node to list of nodes to process in the next dispatch call
	uint searchIndex = atomicCounterIncrement( pNextNodeIndex );
	pSearchNodes[ searchIndex / 4 ][ searchIndex % 4 ] = index;
	
	// if the count of nodes to search increases by the dispatch workgroup size
	// increment also the work group count. this way the upcomding dispatch
	// indirect call knows the count of workgroups to run
	if( searchIndex % dispatchWorkGroupSize == 0 ){
		atomicCounterIncrement( pDispatchWorkGroupCount );
	}
}
