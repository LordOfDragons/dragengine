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
	uint lodFirst;
	uint lodCount;
};


struct sElementTexture{
	uint renderFilter; // erf* (world_element_constants)
	uint pipelineModifiers; // epm* (world_element_constants)
	uint skinTexture; // index into pSkinTextures
	uint instanceIndex; // index into render task shared instance
};


struct sSkinTexture{
	/*
	list of tucs as uint referencing tuc pool index.
	for skin-texture there are 11 pipelines lists each with 11 pipeline types each
	with 8 pipeline modifiers. this is a total of 11*11*8 = 968 pipelines. this can
	be stored as 242 uvec4 with a total of 3872 bytes per entry
	*/
	uvec4 piplines[ 242 ];
};


struct sElementLod{
	float maxError;
	float avgError;
}


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
