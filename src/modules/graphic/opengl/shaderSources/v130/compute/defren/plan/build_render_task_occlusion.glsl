precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_constants.glsl"
#include "v130/shared/defren/plan/world_element_geometry.glsl"
#include "v130/shared/defren/plan/render_task_config.glsl"
#include "v130/shared/defren/plan/render_task.glsl"


UBOLAYOUT_BIND(0) readonly buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};

UBOLAYOUT_BIND(1) readonly buffer ElementCullResult {
	uvec4 pElementCullResult[];
};

UBOLAYOUT_BIND(2) writeonly buffer RenderTask {
	sRenderTask pRenderTask[];
};

#include "v130/shared/defren/plan/element_cull_result_get.glsl"
#include "v130/shared/defren/plan/render_task_set.glsl"


layout( local_size_x=64 ) in;


layout( binding=0, offset=0 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=12 ) uniform atomic_uint pNextIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	#define config pRenderTaskConfig[ 0 ]
	
	uint index = gl_GlobalInvocationID.x;
	if( index >= config.elementGeometryCount ){
		return;
	}
	
	// filter by render task filters. config.renderTaskFilters contains config.renderTaskFilterMask
	uint rfgeometry = pElementGeometries[ index ].renderFilter;
	if( ( rfgeometry & config.renderTaskFilterMask ) != config.renderTaskFilter ){
		return;
	}
	
	// filter by lod is not used for occlusion
	
	// filter by cube face is not used for occlusion
	
	// check if element is visible. cullResult is actually composed of the visibility (bit 8),
	// the cull result flags (bit 0-7) and the lod index (bit 12-15). since though the cull
	// result flags and lod index are not written unless the element is visible the value of
	// cullResult is always 0 if the element is invisible and not 0 if visible
	uint cullResult = getElementCullResult( pElementGeometries[ index ].element );
	if( cullResult == uint( 0 ) ){
		return;
	}
	
	// get next index to store write render task step to
	uint nextIndex = atomicCounterIncrement( pNextIndex );
	
	// if the count of steps increases by the dispatch workgroup size increment also the
	// work group count. this way the upcoming dispatch indirect calls know the count
	// of workgroups to run
	if( nextIndex % dispatchWorkGroupSize == uint( 0 ) ){
		atomicCounterIncrement( pDispatchWorkGroupCount );
	}
	
	// if SSBO is not large enough do not write step. this avoids creating very large SSBOs
	// for storing render steps. before sorting it is checked if the SSBO has been large
	// enough and if not it is enlarged and the task rebuild
	if( nextIndex >= pRenderTask.length() ){
		return;
	}
	
	bool doubleSided = ( rfgeometry & erfDoubleSided ) == erfDoubleSided;
	setRenderTaskStep( nextIndex, 0,
		doubleSided ? config.pipelineDoubleSided : config.pipelineSingleSided,
		0, // empty tuc
		pElementGeometries[ index ].vao,
		pElementGeometries[ index ].instance,
		pElementGeometries[ index ].spbInstance,
		0 ); // flags
}
