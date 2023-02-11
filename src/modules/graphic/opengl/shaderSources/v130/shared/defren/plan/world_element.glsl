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
