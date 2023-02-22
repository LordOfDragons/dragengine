precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_constants.glsl"
#include "v130/shared/defren/plan/world_element_geometry.glsl"
#include "v130/shared/defren/plan/skin_texture.glsl"
#include "v130/shared/defren/plan/render_task_config.glsl"
#include "v130/shared/defren/plan/render_task.glsl"


UBOLAYOUT_BIND(0) readonly buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(3) readonly buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};

UBOLAYOUT_BIND(2) readonly buffer SkinTexturePipelines {
	uvec4 pSkinTexturePipelines[];
};

UBOLAYOUT_BIND(4) writeonly buffer RenderTask {
	sRenderTask pRenderTask[];
};

#include "v130/shared/defren/plan/world_element_geometry_tuc.glsl"
#include "v130/shared/defren/plan/skin_texture_pipeline.glsl"
#include "v130/shared/defren/plan/render_task_set.glsl"
#include "v130/shared/defren/plan/render_task_set_geometry.glsl"


layout( local_size_x=64 ) in;


layout( binding=0, offset=0 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=12 ) uniform atomic_uint pNextIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	if( gl_GlobalInvocationID.x >= pElementGeometryCount ){
		return;
	}
	
	uint index = gl_GlobalInvocationID.x;
	uint elementIndex = pElementGeometries[ index ].element;
	bvec3 cond;
	
	
	uint cullResult = pElement[ elementIndex ].cullResult;
	
	// check if element is visible. cullResult is actually composed of the visibility (bit 8)
	// and the cull result flags (bit 0-7). since though the cull result flags are not
	// written unless the element is visible the value of cullResult is always 0 if the
	// element is invisible and not 0 if visible
	cond.x = cullResult == uint( 0 );
	
	// check geometry matches the lod level selected for the element
	cond.y = pElementGeometries[ index ].lod != pElement[ elementIndex ].lodIndex;
	
	if( any( cond.xy ) ){
		return;
	}
	
	
	uint pipelineBase = pElementGeometries[ index ].pipelineBase;
	uint pipelineList = pipelineBase & uint( 0xff );
	uint pipelineModifier = pipelineBase >> uint( 8 );
	
	// filter by pipeline list
	cond.x = ( pFilterPipelineLists & ( uint( 1 ) << pipelineList ) ) == uint( 0 );
	
	// filter cube face. we are using here the fact that if element is visible the bit 8
	// in the cull mask is set. the same bit is used to indicate if filter cube face has
	// to be used. this way cullResult and pFilterCubeFace can be directly compared.
	// this line is then the same as this calculation given filter has only 1 face bit set:
	// 
	//   ((filter & 0x100) == 0x100) && ((cullResult & 0x3f) & (filter & 0x3f)) == 0
	// 
	cond.y = ( cullResult & pFilterCubeFace ) == uint( 0x100 );
	
	// filter by render task filters. pRenderTaskFilters contains pRenderTaskFilterMask
	cond.z = ( pElementGeometries[ index ].renderFilter & pRenderTaskFilterMask ) != pRenderTaskFilter;
	
	if( any( cond ) ){
		return;
	}
	
	
	// add to render task
	uint nextIndex = atomicCounterIncrement( pNextIndex );
	
	setRenderTaskStepGeometry( nextIndex, index,
		pipelineList, pPipelineType, pPipelineModifier | pipelineModifier,
		cullResult & uint( 0xff ) ); // NOTE masking not necessarily required
	
	// if the count of steps increases by the dispatch workgroup size increment also the
	// work group count. this way the upcoming dispatch indirect calls know the count
	// of workgroups to run
	if( nextIndex % dispatchWorkGroupSize == uint( 0 ) ){
		atomicCounterIncrement( pDispatchWorkGroupCount );
	}
}
