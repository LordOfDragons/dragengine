precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/defines.glsl"
#include "v130/shared/defren/plan/intersect_frustum.glsl"
#include "v130/shared/defren/plan/intersect_gi.glsl"


UBOLAYOUT_BIND(2) readonly buffer SearchNodes {
	uvec4 pSearchNodes[];
};

UBOLAYOUT_BIND(3) readonly buffer Counters {
	uvec3 pWorkGroupSize; // not used
	uint pNodeIndexCount; // from previous run atomic counter pNextNodeIndex
};

UBOLAYOUT_BIND(4) writeonly buffer VisibleElements {
	uvec4 pVisibleElements[];
};


layout( local_size_x=64 ) in;


// buffer stores in the first uvec4 the output of the node search pass. the output
// of the element search pass is stored in the next uvec4. for this reason the
// offset 16 is added so we can reuse the same binding (binding point 3) for input
// and output through different variables. we store the work group size too so
// we can dispatch over the results in later passes
layout( binding=0, offset=16 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=28 ) uniform atomic_uint pNextVisibleIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	// skip outside of parameter space
	if( gl_GlobalInvocationID.x >= pNodeIndexCount ){
		return;
	}
	
	uint nodeIndex = pSearchNodes[ gl_GlobalInvocationID.x ];
	uint first = pWorldOctreeNode[ nodeIndex ].firstNode + pWorldOctreeNode[ nodeIndex ].childNodeCount;
	uint last = first + pWorldOctreeNode[ index ].elementCount;
	uint index;
	
	for( index=first; index<last; index++ ){
		vec3 minExtend = pWorldOctreeData[ index ].minExtend;
		vec3 maxExtend = pWorldOctreeData[ index ].maxExtend;
		uint flags = pWorldOctreeElement[ index ].flags;
		uvec2 layerMask = pWorldOctreeElement[ index ].layerMask;
		bool isLight = ( flags & wodfLight ) != 0;
		bvec4 cond;
		
		// if element does not intersect the frustum nor the GI cascade box skip all content
		// inside. the GI cascade box test is only done for lights
		cond.x = intersectFrustum( minExtend, maxExtend );
		cond.y = isLight && intersectGI( minExtend, maxExtend );
		if( ! any( cond.xy ) ) ){
			continue;
		}
		
		// cull using layer mask if required. components with empty layer mask never match
		// and thus are never culled
		cond.x = pCullLayerMask;
		cond.y = any( notEqual( layerMask, uvec2( 0 ) ); // element.layerMask.IsNotEmpty()
		cond.zw = equal( pLayerMask & layerMask ) == uvec2( 0 ); // pLayerMask.MatchesNot(element.layerMask)
		if( all( cond ) ){
			continue;
		}
		
		// cull using too small filter. applies not for lights
		vec3 center = ( minExtend + maxExtend ) * 0.5;
		float radius = length( maxExtend - minExtend ) * 0.5;
		float dist = ( ( center - pCameraPosition ) * pCameraView ) - radius;
		
		cond.x = isLight;
		cond.y = radius < dist * pErrorScaling;
		if( cond.xy ){
			continue;
		}
		
		// cull dynamic if required. used for components only since only only those
		// set the static flag. if more complex culling is required this can be extended
		if( ( pCullFlags & flags ) != uint( 0 ) ){
			continue;
		}
		
		// add element to found visible elements list
		uint visibleIndex = atomicCounterIncrement( pNextVisibleIndex );
		pVisibleElements[ visibleIndex ] = index;
		
		// if the count of visible elements increases by the dispatch workgroup size
		// increment also the work group count. this way the upcoming dispatch
		// indirect calls know the count of workgroups to run
		if( visibleIndex % dispatchWorkGroupSize == uint( 0 ) ){
			atomicCounterIncrement( pDispatchWorkGroupCount );
		}
	}
}
