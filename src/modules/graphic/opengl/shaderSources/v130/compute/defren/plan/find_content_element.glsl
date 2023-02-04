precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/defines.glsl"
#include "v130/shared/defren/plan/intersect_frustum.glsl"
#include "v130/shared/defren/plan/intersect_gi.glsl"


UBOLAYOUT_BIND(0) readonly buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(1) writeonly buffer VisibleElement {
	uvec4 pVisibleElement[];
};

#ifndef DIRECT_ELEMENTS
UBOLAYOUT_BIND(2) readonly buffer SearchNodes {
	uvec4 pSearchNodes[];
};

UBOLAYOUT_BIND(3) readonly buffer Counters {
	uvec3 pWorkGroupSize; // not used
	uint pNodeIndexCount; // from previous run atomic counter pNextNodeIndex
};
#endif


layout( local_size_x=64 ) in;


// buffer stores in the first uvec4 the output of the node search pass. the output
// of the element search pass is stored in the next uvec4. for this reason the
// offset 16 is added so we can reuse the same binding (binding point 3) for input
// and output through different variables. we store the work group size too so
// we can dispatch over the results in later passes
layout( binding=0, offset=0 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=12 ) uniform atomic_uint pNextVisibleIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	// skip outside of parameter space
	#ifdef DIRECT_ELEMENTS
	if( gl_GlobalInvocationID.x >= pElementCount ){
		return;
	}
	#else
	if( gl_GlobalInvocationID.x >= pNodeIndexCount ){
		return;
	}
	#endif
	
// 	uint nodeIndex = pSearchNodes[ gl_GlobalInvocationID.x / uint( 4 ) ][ gl_GlobalInvocationID.x % uint( 4 ) ];
// 	uint index = pWorldOctreeNode[ nodeIndex ].firstElement;
// 	uint last = index + pWorldOctreeNode[ nodeIndex ].elementCount;
	
	#ifdef DIRECT_ELEMENTS
	uint index = gl_GlobalInvocationID.x;
	#else
	uint ind1 = gl_GlobalInvocationID.x / uint( 2 );
	uint ind2 = ( gl_GlobalInvocationID.x % uint( 2 ) ) * uint( 2 );
	uint index = pSearchNodes[ ind1 ][ ind2 ];
	uint last = pSearchNodes[ ind1 ][ ind2 + 1 ];
	
	for( ; index<last; index++ ){
	#endif
		
		vec3 minExtend = pElement[ index ].minExtend;
		vec3 maxExtend = pElement[ index ].maxExtend;
		uint flags = pElement[ index ].flags;
		uvec2 layerMask = pElement[ index ].layerMask;
		bool isLight = ( flags & wodfLight ) != 0;
		bvec4 cond;
		
		// if element does not intersect the frustum nor the GI cascade box skip all content
		// inside. the GI cascade box test is only done for lights
		cond.x = intersectFrustum( minExtend, maxExtend );
		cond.y = isLight && intersectGI( minExtend, maxExtend );
		if( ! any( cond.xy ) ){
			#ifdef DIRECT_ELEMENTS
			return;
			#else
			continue;
			#endif
		}
		
		// cull using layer mask if required. components with empty layer mask never match
		// and thus are never culled
		cond.x = pCullLayerMask;
		cond.y = any( notEqual( layerMask, uvec2( 0 ) ) ); // element.layerMask.IsNotEmpty()
		cond.zw = equal( pLayerMask & layerMask, uvec2( 0 ) ); // pLayerMask.MatchesNot(element.layerMask)
		if( all( cond ) ){
			#ifdef DIRECT_ELEMENTS
			return;
			#else
			continue;
			#endif
		}
		
		// cull using too small filter. applies not for lights
		vec3 center = ( minExtend + maxExtend ) * 0.5;
		float radius = length( maxExtend - minExtend ) * 0.5;
		float dist = dot( center - pCameraPosition, pCameraView ) - radius;
		
		cond.x = ! isLight;
		cond.y = radius < dist * pErrorScaling;
		if( all( cond.xy ) ){
			#ifdef DIRECT_ELEMENTS
			return;
			#else
			continue;
			#endif
		}
		
		// cull dynamic if required. used for components only since only only those
		// set the static flag. if more complex culling is required this can be extended
		if( ( pCullFlags & flags ) != uint( 0 ) ){
			#ifdef DIRECT_ELEMENTS
			return;
			#else
			continue;
			#endif
		}
		
		// add element to found visible elements list
		uint visibleIndex = atomicCounterIncrement( pNextVisibleIndex );
		pVisibleElement[ visibleIndex / uint( 4 ) ][ visibleIndex % uint( 4 ) ] = index;
		
		// if the count of visible elements increases by the dispatch workgroup size
		// increment also the work group count. this way the upcoming dispatch
		// indirect calls know the count of workgroups to run
		if( visibleIndex % dispatchWorkGroupSize == uint( 0 ) ){
			atomicCounterIncrement( pDispatchWorkGroupCount );
		}
		
	#ifndef DIRECT_ELEMENTS
	}
	#endif
}
