precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_constants.glsl"

UBOLAYOUT_BIND(0) buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(1) readonly buffer VisibleElement {
	uvec4 pVisibleElement[];
};

UBOLAYOUT_BIND(3) readonly buffer Counters {
	uvec3 pWorkGroupSize; // not used
	uint pNodeIndexCount; // from previous run atomic counter pNextNodeIndex
};


#ifndef CLEAR_CULL_RESULT
	#include "v130/shared/defren/plan/find_config.glsl"
	#include "v130/shared/defren/plan/calc_lod.glsl"
#endif


layout( local_size_x=64 ) in;


void main( void ){
	if( gl_GlobalInvocationID.x >= pNodeIndexCount ){
		return;
	}
	
	uint visInd1 = gl_GlobalInvocationID.x / uint( 4 );
	uint visInd2 = gl_GlobalInvocationID.x % uint( 4 );
	uint visibleElement = pVisibleElement[ visInd1 ][ visInd2 ];
	
	uint index = visibleElement & uint( 0xffffff );
	
	#ifdef CLEAR_CULL_RESULT
		pElement[ index ].cullResult = uint( 0 );
	#else
		pElement[ index ].cullResult = ( visibleElement >> uint( 24 ) ) | ecrVisible;
		pElement[ index ].lodIndex = calcLod( index );
	#endif
}
