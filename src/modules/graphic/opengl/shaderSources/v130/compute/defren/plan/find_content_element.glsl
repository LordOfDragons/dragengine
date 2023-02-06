precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/find_config.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_flags.glsl"
#include "v130/shared/defren/plan/intersect_frustum.glsl"
#include "v130/shared/defren/plan/intersect_gi.glsl"

#if defined CULL_SKY_LIGHT_FRUSTUM && ! defined CULL_SKY_LIGHT_GI
	#include "v130/shared/defren/plan/intersect_hull.glsl"
	#include "v130/shared/defren/plan/intersect_light_frustum.glsl"
	#include "v130/shared/defren/plan/split_mask.glsl"
#endif


UBOLAYOUT_BIND(0) readonly buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(1) writeonly buffer VisibleElement {
	uvec4 pVisibleElement[];
};

UBOLAYOUT_BIND(2) writeonly buffer VisibleElementFlags {
	uvec4 pVisibleElementFlags[];
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


#ifdef DIRECT_ELEMENTS
	#define SKIP_ELEMENT return;
#else
	#define SKIP_ELEMENT continue;
#endif


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
		bool isLight = ( flags & efLight ) != 0;
		bvec4 cond;
		
		// if element does not intersect the frustum nor the GI cascade box skip all content
		// inside. the GI cascade box test is only done for lights
		#ifdef CULL_VIEW_FRUSTUM
			cond.x = intersectFrustum( minExtend, maxExtend );
			cond.y = isLight && intersectGI( minExtend, maxExtend );
			if( ! any( cond.xy ) ){
				SKIP_ELEMENT
			}
		#endif
		
		#if defined CULL_SKY_LIGHT_FRUSTUM || defined CULL_SKY_LIGHT_GIBOX
			vec3 che = ( maxExtend - minExtend ) * 0.5;
			vec3 nhe = vec3( dot( pShadowAxis[ 0 ], che ), dot( pShadowAxis[ 1 ], che ), dot( pShadowAxis[ 2 ], che ) );
			
			#ifdef CULL_SKY_LIGHT_FRUSTUM
				vec3 npos = pMatrixLightSpace * vec4( ( minExtend + maxExtend ) * vec3( 0.5 ), 1 );
				
				float adjustdist = ( pLightShaftFar - npos.z ) * 0.5;
				nhe.z += abs( adjustdist );
				npos.z += adjustdist;
				
				// test if the box projected to the shadow map plane hits the frustum
				// hull projected to the shadow map plane
				cond.x = notIntersectHull( npos.xy, nhe.xy );
				
				// test if the light space box shaft hits the light space frustum
				cond.y = notIntersectLightFrustum( npos, nhe );
				
				if( any( cond.xy ) ){
					SKIP_ELEMENT
				}
			#endif
			
			#ifdef CULL_SKY_LIGHT_GIBOX
				cond.xy = greaterThan( nhe.xy * vec2( 2 ), pSplitSizeThreshold[ 0 ] );
				
				if( all( cond.xy ) ){
					SKIP_ELEMENT
				}
			#endif
		#endif
		
		// cull using layer mask if required. components with empty layer mask never match
		// and thus are never culled
		cond.x = pCullLayerMask;
		cond.y = any( notEqual( layerMask, uvec2( 0 ) ) ); // element.layerMask.IsNotEmpty()
		cond.zw = equal( pLayerMask & layerMask, uvec2( 0 ) ); // pLayerMask.MatchesNot(element.layerMask)
		if( all( cond ) ){
			SKIP_ELEMENT
		}
		
		// cull using too small filter. applies not for lights
		#ifdef CULL_TOO_SMALL
			vec3 center = ( minExtend + maxExtend ) * 0.5;
			float radius = length( maxExtend - minExtend ) * 0.5;
			float dist = dot( center - pCameraPosition, pCameraView ) - radius;
			
			cond.x = ! isLight;
			cond.y = radius < dist * pErrorScaling;
			if( all( cond.xy ) ){
				SKIP_ELEMENT
			}
		#endif
		
		// cull dynamic if required. used for components only since only only those
		// set the static flag. if more complex culling is required this can be extended
		if( ( pCullFlags & flags ) != uint( 0 ) ){
			SKIP_ELEMENT
		}
		
		// add element to found visible elements list
		uint visibleIndex = atomicCounterIncrement( pNextVisibleIndex );
		uint visInd1 = visibleIndex / uint( 4 );
		uint visInd2 = visibleIndex % uint( 4 );
		pVisibleElement[ visInd1 ][ visInd2 ] = index;
		
		#ifdef CULL_SKY_LIGHT_FRUSTUM
			pVisibleElementFlags[ visInd1 ][ visInd2 ] = calcSplitMask( npos, nhe );
		#endif
		#ifdef CULL_SKY_LIGHT_GIBOX
			pVisibleElementFlags[ visInd1 ][ visInd2 ] = uint( 1 );
		#endif
		
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
