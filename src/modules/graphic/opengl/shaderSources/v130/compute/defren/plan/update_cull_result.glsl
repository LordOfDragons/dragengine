precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_constants.glsl"
#include "v130/shared/defren/plan/counter.glsl"

#ifndef CLEAR_CULL_RESULT
UBOLAYOUT_BIND(0) readonly buffer Element {
	sElement pElement[];
};
#endif

UBOLAYOUT_BIND(1) readonly buffer VisibleElement {
	uvec4 pVisibleElement[];
};

UBOLAYOUT_BIND(2) readonly buffer Counters {
	sCounter pVisibleElementCounter;
};

UBOLAYOUT_BIND(3) writeonly buffer ElementCullResult {
	uvec4 pElementCullResult[];
};


#ifndef CLEAR_CULL_RESULT
	#include "v130/shared/defren/plan/find_config.glsl"
	
	#ifdef WITH_CALC_LOD
		#include "v130/shared/defren/plan/calc_lod.glsl"
	#endif
#endif

#include "v130/shared/defren/plan/element_cull_result_set.glsl"


layout( local_size_x=64 ) in;


void main( void ){
	if( gl_GlobalInvocationID.x >= pVisibleElementCounter.counter ){
		return;
	}
	
	uint visInd1 = gl_GlobalInvocationID.x / uint( 4 );
	uint visInd2 = gl_GlobalInvocationID.x % uint( 4 );
	uint visibleElement = pVisibleElement[ visInd1 ][ visInd2 ];
	
	uint elementIndex = visibleElement & uint( 0xffffff );
	
	uint cullResult = uint( 0 );
	
	#ifndef CLEAR_CULL_RESULT
		uint flags = visibleElement >> uint( 24 );
		uint lod;
		
		#ifdef WITH_CALC_LOD
			lod = calcLod( elementIndex );
		#else
			lod = uint( 0 );
		#endif
		
		cullResult = composeElementCullResult( flags, lod );
	#endif
	
	setElementCullResult( elementIndex, cullResult );
}
