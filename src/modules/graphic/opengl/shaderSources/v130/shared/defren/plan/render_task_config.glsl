
UBOLAYOUT uniform RenderTaskConfig{
	// count of element geometries
	uint pElementGeometryCount;
	
	// filter by cube face. value is composed like this:
	// - bit 0-5: mask of cube faces. only one bit has to be set
	// - bit 8: enable filter by cube face
	uint pFilterCubeFace;
	
	// filter by render task filters.
	uint pRenderTaskFilters; // filters & filterMask
	uint pRenderTaskFilterMask;
	
	// filter by pipeline list. mask of bits representing epl* values (1 << epl*)
	uint pFilterPipelineLists;
	
	// pipeline type
	uint pPipelineType;
	
	// pipeline modifiers
	uint pPipelineModifiers;
};
