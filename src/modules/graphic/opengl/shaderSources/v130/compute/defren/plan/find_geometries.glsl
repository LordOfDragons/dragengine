precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_constants.glsl"
#include "v130/shared/defren/plan/world_element_geometry.glsl"


UBOLAYOUT_BIND(0) readonly buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};

UBOLAYOUT_BIND(1) readonly buffer ElementCullResult {
	uvec4 pElementCullResult[];
};

UBOLAYOUT_BIND(2) writeonly buffer VisibleGeometry {
	uvec4 pVisibleGeometry[];
};


#include "v130/shared/defren/plan/element_cull_result_get.glsl"


uniform uint pGeometryCount;


layout( local_size_x=64 ) in;


layout( binding=0, offset=16 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=28 ) uniform atomic_uint pNextVisibleIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	if( gl_GlobalInvocationID.x >= pGeometryCount ){
		return;
	}
	
	uint index = gl_GlobalInvocationID.x;
	uint elementIndex = pElementGeometries[ index ].element;
	
	
	bvec2 cond;
	uint cullResult = getElementCullResult( elementIndex );
	
	// check if element is visible. cullResult is actually composed of the visibility (bit 8),
	// the cull result flags (bit 0-7) and the lod index (bit 12-15). since though the cull
	// result flags and lod index are not written unless the element is visible the value of
	// cullResult is always 0 if the element is invisible and not 0 if visible
	cond.x = cullResult == uint( 0 );
	
	// check geometry matches the lod level selected for the element
	cond.y = pElementGeometries[ index ].lod != elementCullResultGetLodIndex( cullResult );
	
	if( any( cond.xy ) ){
		return;
	}
	
	// add geometry to list of visible geometries
	uint visibleIndex = atomicCounterIncrement( pNextVisibleIndex );
	uint visInd1 = visibleIndex / uint( 4 );
	uint visInd2 = visibleIndex % uint( 4 );
	
	pVisibleGeometry[ visInd1 ][ visInd2 ] = index | ( cullResult << uint( 24 ) );
	
	// if the count of visible elements increases by the dispatch workgroup size
	// increment also the work group count. this way the upcoming dispatch
	// indirect calls know the count of workgroups to run
	if( visibleIndex % dispatchWorkGroupSize == uint( 0 ) ){
		atomicCounterIncrement( pDispatchWorkGroupCount );
	}
}