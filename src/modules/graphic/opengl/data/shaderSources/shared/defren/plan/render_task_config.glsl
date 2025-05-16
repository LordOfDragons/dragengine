
struct sRenderTaskConfig{
	// count of element geometries
	uint elementGeometryCount;
	
	// filter by cube face. value is composed like this:
	// - bit 0-5: mask of cube faces. only one bit has to be set
	// - bit 8: enable filter by cube face
	uint filterCubeFace;
	
	// filter by render task filters.
	uint renderTaskFilter; // filter & filterMask
	uint renderTaskFilterMask;
	
	// filter by pipeline list. mask of bits representing epl* values (1 << epl*)
	uint filterPipelineLists;
	
	// pipeline type
	uint pipelineType;
	
	// pipeline modifiers
	uint pipelineModifier;
	
	// for occlusion rendering using WITH_OCCLUSION
	uint pipelineDoubleSided;
	uint pipelineSingleSided;
};

#define MAX_RENDER_TASK_CONFIG 8

UBOLAYOUT_BIND(0) uniform RenderTaskConfig {
	sRenderTaskConfig pRenderTaskConfig[ MAX_RENDER_TASK_CONFIG ];
};
