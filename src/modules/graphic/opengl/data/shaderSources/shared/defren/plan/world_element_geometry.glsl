
// element geometry. world compute elements add geometries for each group of geometry
// to render. each geometry composes of texture information and geometry information.
// texture information is used for filtering and defines the pipeline to use for
// renderring. the geometry information defines the vao, instance and optional sub
// instance to use for rendering. furthermore the element and lod level is stored this
// geometry information belongs to.

struct sElementGeometry{
	// index of element this geometry information belongs to
	uint element;
	
	// index of lod level this geometry information belongs to.
	// NOTE fits into 8-bit uint
	uint lod;
	
	// filter used for building render tasks. use erf* values from
	// world_element_constants.glsl as flags. set to 0 if no valid
	// skin texture is found and the geometry should be skipped
	// NOTE fits into 16-bit uint
	uint renderFilter;
	
	// index into pSkinTextures. used to look up the pipeline to use
	// for the combination of pipelineBase and the pipeline type used
	// while building the render task
	// NOTE fits into 16-bit uint
	uint skinTexture;
	
	// pipeline lists and pipeline modifier to use as base values.
	// use values epl* (from world_element_constants.glsl) for pipeline list
	// and epm* (from world_element_constants.glsl) for modifiers.
	// the modifiers is OR-combined with the modifiers used for building
	// the render task. the value is composed like this:
	// - bits 0..8: pipeline list (epl*)
	// - bits 8..16: pipeline modifier (epm*)
	// NOTE fits into 16-bit uint
	uint pipelineBase;
	
	// index of shared VAO to use for renderirng. this value is typically
	// stored in deoglModelLOD but can be also unique per element
	// NOTE maybe fits into 16-bit
	uint vao;
	
	// render task shared instance index. this value is typically
	// stored in deoglModelLOD but can be also unique per element
	uint instance;
	
	// shared parameter block instance index starting at index 1.
	// 0 means no spb instance is used.
	uint spbInstance;
	
	// texture unit configurations. 8 16-bit tucs are stored compacted
	// into 4 32-bit values.
	uvec4 tucs;
};


// define this somewhere. variable name pElementGeometries has to stay intact
/*
UBOLAYOUT_BIND(3) readonly buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};
*/
