precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/plan/world_element.glsl"
#include "shared/defren/plan/world_element_constants.glsl"
#include "shared/defren/plan/world_element_geometry.glsl"


UBOLAYOUT_BIND(0) readonly buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};

UBOLAYOUT_BIND(1) readonly buffer ElementCullResult {
	uvec4 pElementCullResult[];
};

UBOLAYOUT_BIND(2) writeonly restrict buffer VisibleGeometry {
	uvec4 pVisibleGeometry[];
};


#include "shared/defren/plan/element_cull_result_get.glsl"


uniform uint pGeometryCount;


layout( local_size_x=64 ) in;


// deoglRenderCompute::ecVisibleGeometries = 0
layout( binding=0, offset=0 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=12 ) uniform atomic_uint pNextVisibleIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	uint index = gl_GlobalInvocationID.x;
	if( index >= pGeometryCount ){
		return;
	}
	
	// if render filter is 0 the geometry is disabled and has to be ignore. this check is
	// important since disabled geometry can have invalid element index pointing outside
	// the range of pElementCullResult
	uint rfgeometry = pElementGeometries[ index ].renderFilter;
	if( rfgeometry == uint( 0 ) ){
		return;
	}
	
	// now it is safe to check the element cull result
	uint elementIndex = pElementGeometries[ index ].element;
	
	uint cullResult = getElementCullResult( elementIndex );
	
	// check if element is visible. cullResult is actually composed of the visibility (bit 8),
	// the cull result flags (bit 0-7) and the lod index (bit 12-15). since though the cull
	// result flags and lod index are not written unless the element is visible the value of
	// cullResult is always 0 if the element is invisible and not 0 if visible
	if( cullResult == uint( 0 ) ){
		return;
	}
	
	// check geometry matches the lod level selected for the element
	#ifdef WITH_OCCLUSION
		if( ( rfgeometry & erfOcclusion ) != erfOcclusion
		&& pElementGeometries[ index ].lod != elementCullResultGetLodIndex( cullResult ) ){
			return;
		}
	#else
		if( pElementGeometries[ index ].lod != elementCullResultGetLodIndex( cullResult ) ){
			return;
		}
	#endif
	
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
