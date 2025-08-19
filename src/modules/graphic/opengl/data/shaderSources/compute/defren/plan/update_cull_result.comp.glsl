#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/plan/world_element.glsl"
#include "shared/defren/plan/world_element_constants.glsl"
#include "shared/defren/plan/counter.glsl"

// !ClearCullResult
UBOLAYOUT_BIND(0) readonly buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(1) readonly buffer VisibleElement {
	uvec4 pVisibleElement[];
};

UBOLAYOUT_BIND(2) readonly buffer Counters {
	sCounter pVisibleElementCounter;
};

UBOLAYOUT_BIND(3) writeonly restrict buffer ElementCullResult {
	uvec4 pElementCullResult[];
};


UNIFORM_BIND(0) uniform uint pLodLayer;


// !ClearCullResult
#include "shared/defren/plan/find_config.glsl"

// !ClearCullResult and WithCalcLod
#include "shared/defren/plan/calc_lod.glsl"

#include "shared/defren/plan/element_cull_result_set.glsl"


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
	
	if(!ClearCullResult){
		uint flags = visibleElement >> uint( 24 );
		uint lod = uint(0);
		
		if(WithCalcLod){
			lod = calcLod(elementIndex);
		}
		
		cullResult = composeElementCullResult(flags, lod);
	}
	
	setElementCullResult( elementIndex, cullResult );
}
