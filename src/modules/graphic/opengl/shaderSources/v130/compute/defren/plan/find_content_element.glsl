precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/find_config.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_constants.glsl"

#ifdef CULL_VIEW_FRUSTUM
	#include "v130/shared/defren/plan/intersect_frustum.glsl"
#endif

#include "v130/shared/defren/plan/intersect_gi.glsl"

#ifdef CULL_SKY_LIGHT_FRUSTUM
	#include "v130/shared/defren/plan/intersect_hull.glsl"
	#include "v130/shared/defren/plan/intersect_light_frustum.glsl"
	#include "v130/shared/defren/plan/split_mask.glsl"
#endif

UBOLAYOUT_BIND(0) readonly buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer VisibleElement {
	uvec4 pVisibleElement[];
};

#ifdef WRITE_CULL_RESULT
UBOLAYOUT_BIND(2) writeonly restrict buffer ElementCullResult {
	uvec4 pElementCullResult[];
};
#endif


#ifdef WRITE_CULL_RESULT
	#include "v130/shared/defren/plan/calc_lod.glsl"
	#include "v130/shared/defren/plan/element_cull_result_set.glsl"
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
	if( gl_GlobalInvocationID.x >= pElementCount ){
		return;
	}
	
	uint index = gl_GlobalInvocationID.x;
	
	vec3 minExtend = pElement[ index ].minExtend;
	vec3 maxExtend = pElement[ index ].maxExtend;
	uint flags = pElement[ index ].flags;
	uvec2 layerMask = pElement[ index ].layerMask;
	bool isLight = ( flags & efLight ) != uint( 0 );
	bvec4 cond;
	
	// if element does not intersect the frustum nor the GI cascade box skip all content
	// inside. the GI cascade box test is only done for lights
	#ifdef CULL_VIEW_FRUSTUM
		cond.x = intersectFrustum( minExtend, maxExtend );
		cond.y = isLight && intersectGI( minExtend, maxExtend );
		if( ! any( cond.xy ) ){
			#ifdef WRITE_CULL_RESULT
				setElementCullResult( index, uint( 0 ) );
			#endif
			return;
		}
	#endif
	
	#if defined CULL_SKY_LIGHT_FRUSTUM || defined CULL_SKY_LIGHT_GIBOX
		vec3 che = ( maxExtend - minExtend ) * vec3( 0.5 );
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
				#ifdef WRITE_CULL_RESULT
					setElementCullResult( index, uint( 0 ) );
				#endif
				return;
			}
		#endif
		
		#ifdef CULL_SKY_LIGHT_GIBOX
			cond.x = notIntersectGI( minExtend, maxExtend );
			cond.yz = lessThanEqual( nhe.xy * vec2( 2 ), pSplitSizeThreshold[ 0 ] );
			
			if( any( cond.xyz ) ){
				#ifdef WRITE_CULL_RESULT
					setElementCullResult( index, uint( 0 ) );
				#endif
				return;
			}
		#endif
	#endif
	
	// cull using layer mask if required. components with empty layer mask never match
	// and thus are never culled
	cond.x = pCullLayerMask;
	cond.y = any( notEqual( layerMask, uvec2( 0 ) ) ); // element.layerMask.IsNotEmpty()
	cond.zw = equal( pLayerMask & layerMask, uvec2( 0 ) ); // pLayerMask.MatchesNot(element.layerMask)
	if( all( cond ) ){
		#ifdef WRITE_CULL_RESULT
			setElementCullResult( index, uint( 0 ) );
		#endif
		return;
	}
	
	// cull using too small filter. applies not for lights
	#ifdef CULL_TOO_SMALL
		vec3 center = ( minExtend + maxExtend ) * 0.5;
		float radius = length( maxExtend - minExtend ) * 0.5;
		float dist = dot( center - pCameraPosition, pCameraView ) - radius;
		
		cond.x = ! isLight;
		cond.y = radius < dist * pErrorScaling;
		if( all( cond.xy ) ){
			#ifdef WRITE_CULL_RESULT
				setElementCullResult( index, uint( 0 ) );
			#endif
			return;
		}
	#endif
	
	// cull dynamic if required. used for components only since only only those
	// set the static flag. if more complex culling is required this can be extended
	if( ( pCullFlags & flags ) != uint( 0 ) ){
		#ifdef WRITE_CULL_RESULT
			setElementCullResult( index, uint( 0 ) );
		#endif
		return;
	}
	
	// calculate result flags
	uint resultFlags = uint( 0 );
	#ifdef CULL_SKY_LIGHT_FRUSTUM
		resultFlags = calcSplitMask( npos, nhe );
	#endif
	
	// write cull result
	#ifdef WRITE_CULL_RESULT
		setElementCullResult( index, composeElementCullResult( resultFlags, calcLod( index ) ) );
	#endif
	
	// add element to found visible elements list
	uint visibleIndex = atomicCounterIncrement( pNextVisibleIndex );
	uint visInd1 = visibleIndex / uint( 4 );
	uint visInd2 = visibleIndex % uint( 4 );
	
	#ifdef CULL_SKY_LIGHT_FRUSTUM
		index |= resultFlags << uint( 24 );
	#endif
	
	pVisibleElement[ visInd1 ][ visInd2 ] = index;
	
	// if the count of visible elements increases by the dispatch workgroup size
	// increment also the work group count. this way the upcoming dispatch
	// indirect calls know the count of workgroups to run
	if( visibleIndex % dispatchWorkGroupSize == uint( 0 ) ){
		atomicCounterIncrement( pDispatchWorkGroupCount );
	}
}
