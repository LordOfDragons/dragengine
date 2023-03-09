precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"
#include "v130/shared/defren/plan/world_element_constants.glsl"
#include "v130/shared/defren/plan/world_element_geometry.glsl"

#ifndef WITH_OCCLUSION
	#include "v130/shared/defren/plan/skin_texture.glsl"
#

#include "v130/shared/defren/plan/render_task_config.glsl"
#include "v130/shared/defren/plan/render_task.glsl"


UBOLAYOUT_BIND(0) readonly buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};

#ifndef WITH_OCCLUSION
UBOLAYOUT_BIND(1) readonly buffer SkinTexturePipelines {
	uvec4 pSkinTexturePipelines[];
};
#endif

UBOLAYOUT_BIND(2) readonly buffer VisibleGeometry {
	uvec4 pVisibleGeometry[];
};

struct sCounter {
	uvec3 workGroupSize;
	uint counter;
};

UBOLAYOUT_BIND(3) readonly buffer Counters {
	sCounter pVisibleGeometryCounters[ 2 ];
};

UBOLAYOUT_BIND(4) writeonly buffer RenderTask {
	sRenderTask pRenderTask[];
};

#ifndef WITH_OCCLUSION
	#include "v130/shared/defren/plan/world_element_geometry_tuc.glsl"
	#include "v130/shared/defren/plan/skin_texture_pipeline.glsl"
#endif

#include "v130/shared/defren/plan/render_task_set.glsl"

#ifndef WITH_OCCLUSION
	#include "v130/shared/defren/plan/render_task_set_geometry.glsl"
#endif


uniform uint pPass;


layout( local_size_x=64 ) in;


layout( binding=0, offset=0 ) uniform atomic_uint pDispatchWorkGroupCount;
layout( binding=0, offset=12 ) uniform atomic_uint pNextIndex;

const uint dispatchWorkGroupSize = uint( 64 );


void main( void ){
	#define config pRenderTaskConfig[ pPass ]
	
	if( gl_GlobalInvocationID.x >= pVisibleGeometryCounters[ 1 ].counter ){
		return;
	}
	
	uint visibleIndex = gl_GlobalInvocationID.x;
	
	uint visInd1 = visibleIndex / uint( 4 );
	uint visInd2 = visibleIndex % uint( 4 );
	uint visibleGeometry = pVisibleGeometry[ visInd1 ][ visInd2 ];
	
	uint geometryIndex = visibleGeometry & uint( 0xffffff );
	
	#ifndef WITH_OCCLUSION
		uint pipelineBase = pElementGeometries[ geometryIndex ].pipelineBase;
		uint pipelineList = pipelineBase & uint( 0xff );
		uint pipelineModifier = pipelineBase >> uint( 8 );
	#endif
	
	// filter by pipeline list
	bvec3 cond;
	#ifdef WITH_OCCLUSION
		cond.x = false;
	#else
		cond.x = ( config.filterPipelineLists & ( uint( 1 ) << pipelineList ) ) == uint( 0 );
	#endif
	
	// filter cube face. we are using here the fact that if element is visible the bit 8
	// in the cull mask is set. the same bit is used to indicate if filter cube face has
	// to be used. this way cullResult and config.filterCubeFace can be directly compared.
	// this line is then the same as this calculation given filter has only 1 face bit set:
	// 
	//   ((filter & 0x100) == 0x100) && ((cullResult & 0x3f) & (filter & 0x3f)) == 0
	//
	uint cullFlags = visibleGeometry >> uint( 24 );
	cond.y = ( ( cullFlags | ecrVisible ) & config.filterCubeFace ) == ecrVisible;
	
	// filter by render task filters. config.renderTaskFilters contains config.renderTaskFilterMask
	cond.z = ( pElementGeometries[ geometryIndex ].renderFilter & config.renderTaskFilterMask ) != config.renderTaskFilter;
	
	if( any( cond ) ){
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
	
	#ifdef WITH_OCCLUSION
		bool doubleSided = ( pElementGeometries[ geometryIndex ].renderFilter & erfDoubleSided ) == erfDoubleSided;
		setRenderTaskStep( nextIndex, pPass,
			doubleSided ? config.pipelineDoubleSided : config.pipelineSingleSided,
			0, // empty tuc
			pElementGeometries[ geometry ].vao,
			pElementGeometries[ geometry ].instance,
			pElementGeometries[ geometry ].spbInstance,
			specialFlags );
	#else
		setRenderTaskStepGeometry( nextIndex, pPass, geometryIndex, pipelineList,
			config.pipelineType, config.pipelineModifier | pipelineModifier, cullFlags );
	#endif
}
