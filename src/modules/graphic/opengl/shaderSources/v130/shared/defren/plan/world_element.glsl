// element
struct sElement{
	vec3 minExtend;
	uint flags;
	
	vec3 maxExtend;
	uint updateIndex; // only for use by updating
	
	// layer mask split into upper 32 bits (x) and lower 32 bits (y)
	uvec2 layerMask;
	
	// texture information
	uint textureFirst;
	uint textureCount;
	
	// lod informatin if present
	// NOTE lod information is per ModelLOD and thus shared
	uint lodFirst;
	uint lodCount;
};


struct sElementTexture{
	uint renderFilter; // erf* (world_element_constants)
	uint skinTexture; // index into pSkinTextures
	uvec2 tucSortOrder; // sort order to use for TUC sorting (geometry and depth)
	                    // NOTE component has most TUC variations:
	                    //      - 8 TUC types for non-outline
	                    //      - 3 TUC types for outline
	                    //      all other element types have less with those that exit
	                    //      to match one of the component TUC types
	                    //      so basically a TUC array of 11 would cover all bases
	                    // NOTE TUCs should be possible to do with 16-bit only. this way
	                    //      two TUCs can be stored in one uint. in the future the
	                    //      TUCs should be reduced in count anyway
	
	uint pipelineBase; // epm* and epl* (world_element_constants) combined into one value
	                   // (epl* << 11) | pipelineModifiers
	                   // NOTE with skin texture storing the entire table we have to store
	                   //      this value tightly again like this:
	                   //        index = piplist * 18 * 8 + piptype * 8 + pipmod
	                   //      this yields the 32-bit index. this has to be then modified
	                   //      to access the 16-bit value
	uint sharedInstance; // render task shared instance index
	                     // NOTE this is per LOD so it can not be here!
	                     // NOTE value is stored in ModelLOD not ComponentLOD
	uint spbInstance; // shared parameter block instance index
};


struct sSkinTexture{
	/*
	for skin-texture there are 11 pipelines lists each with 18 pipeline types each
	with 8 pipeline modifiers. this is a total of 11*18*8 = 1584 pipelines. this can
	be stored as 396 uvec4 with a total of 6336 bytes per entry. With for example
	500 skin textures this results in over 3.2MB for these tables.
	
	pipelines between pipeline lists, pipeline types and pipeline modifiers are in
	general not compatible. the large tables in GPU can thus be avoided.
	
	sort order:
	- pipeline lists
	- pipeline type
	- pipeline modifiers
	
	this sorting results now in groups containing potentially shared pipelines.
	pipelies in skin textures have though a certain relationship. in particular
	if we sort by the most granular pipeline type (typically geometry) we get a
	sorting which is also usable for coarse grained pipeline types. this works
	if during rendering the active pipeline is tracked and switched only if an
	upcoming render step uses a different one. this should still result in an
	efficient rendering without redundant pipeline switches. the sort order value
	below is just the pipeline index of the most fine grained pipeline but could
	be any number which has the same properties.
	
	the sorting thus becomes this:
	- skin texture sort order
	- element texture sort order
	- element instance index
	
	with element texture sort order we have the same problem as with skin texture
	pipelines. each pipeline type has potentially a different tuc. thus 18 tucs
	are possible for elements but in general less than these are used. here too
	one tuc is the finest grained one (typically geometry). here too we can use
	the tuc index of the finest grained one as sort order. this way another look
	up table can be avoided by tracking during rendering the last activated tuc.
	
	to improve the sorting and reducing memory consumption the pipeline list,
	pipeline type and pipeline modifiers are combined into one value like this:
	- sortPipeline = (pipelineList << 11) | (pipelineType << 5) | pipelineModifiers
	
	in glsl this can be handled using bitfieldInsert and bitfieldExtract. the
	offsets are due to the bit requirements:
	- pipeline lists: 11 values => 4 bits
	- pipeline type: 18 values => 5 bits
	- pipeline modifiers: 8 values => 3 bits
	
	this yields a total of 12 bits. we want to keep this under 16 bits so we
	can potentially use 16-bit integer. to avoid future changes the bits are
	extended a bit like this:
	- pipeline lists: 5 bits
	- pipeline type: 6 bits
	- pipeline modifiers: 5 bits
	
	*/
	uvec4 sortOrder; // two components are used for each skin texture. the first component for
	                 // geometry type pipeline types and the second for depth type ones
};


struct sElementLod{
	float maxError;
	float avgError;
};


#if 0
struct sWorldOctreeNode{
	vec3 minExtend;
	uint firstNode;
	
	vec3 maxExtend;
	uint childNodeCount;
	
	uint firstElement;
	uint elementCount;
};
#endif
