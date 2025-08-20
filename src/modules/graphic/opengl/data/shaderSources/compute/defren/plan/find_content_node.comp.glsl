#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/plan/find_config.glsl"
#include "shared/defren/plan/world_element.glsl"
#include "shared/defren/plan/world_element_constants.glsl"
#include "shared/defren/plan/intersect_frustum.glsl"
#include "shared/defren/plan/intersect_gi.glsl"


UBOLAYOUT_BIND(0) readonly buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(1) readonly buffer WorldOctreeNode {
	sWorldOctreeNode pWorldOctreeNode[];
};

UBOLAYOUT_BIND(2) writeonly restrict buffer SearchNodes {
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
layout( binding=0, offset=16 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=28 ) uniform atomic_uint pNextNodeIndex;

const uint dispatchWorkGroupSize = uint( 64 );
const uint rangeSize = uint( 8 );

void main( void ){
	// skip outside of parameter space
	if( gl_GlobalInvocationID.x >= pNodeCount ){
		return;
	}
	
	uint index = gl_GlobalInvocationID.x;
	
	// if the node has no elements skip it
	uint elementCount = pWorldOctreeNode[ index ].elementCount;
	if( elementCount == 0 ){
		return;
	}
	
	// if node does not intersect the frustum nor the GI cascade box skip all content
	// inside. the GI cascade box test can be easily disabled by settings pGIMinExtend
	// equal to pGIMaxExtend preferably outside the frustum box
	vec3 minExtend = pWorldOctreeNode[ index ].minExtend;
	vec3 maxExtend = pWorldOctreeNode[ index ].maxExtend;
	
	if( notIntersectFrustum( minExtend, maxExtend ) && notIntersectGI( minExtend, maxExtend ) ){
		return;
	}
	
	// add search range to list to process in the next dispatch call
	uint searchIndex = atomicCounterIncrement( pNextNodeIndex );
	uint ind1 = searchIndex / uint( 2 );
	uint ind2 = ( searchIndex % uint( 2 ) ) * uint( 2 );
	
	uint first = pWorldOctreeNode[ index ].firstElement;
	
	pSearchNodes[ ind1 ][ ind2 ] = first;
	pSearchNodes[ ind1 ][ ind2 + 1 ] = first + elementCount;
	
	// if the count of nodes to search increases by the dispatch workgroup size
	// increment also the work group count. this way the upcomding dispatch
	// indirect call knows the count of workgroups to run
	if( searchIndex % dispatchWorkGroupSize == uint( 0 ) ){
		atomicCounterIncrement( pDispatchWorkGroupCount );
	}
	
	/*
	uint first = pWorldOctreeNode[ index ].firstElement;
	uint last = first + pWorldOctreeNode[ index ].elementCount;
	
	while( first < last ){
		uint nextFirst = min( first + rangeSize, last );
		
		uint searchIndex = atomicCounterIncrement( pNextNodeIndex );
		uint ind1 = searchIndex / uint( 2 );
		uint ind2 = ( searchIndex % uint( 2 ) ) * uint( 2 );
		
		pSearchNodes[ ind1 ][ ind2 ] = first;
		pSearchNodes[ ind1 ][ ind2 + 1 ] = nextFirst;
		
		if( searchIndex % dispatchWorkGroupSize == uint( 0 ) ){
			atomicCounterIncrement( pDispatchWorkGroupCount );
		}
		
		first = nextFirst;
	}
	*/
}
