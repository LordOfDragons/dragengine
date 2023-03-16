// element
struct sElement{
	vec3 minExtend;
	uint flags;
	
	vec3 maxExtend;
	uint updateIndex; // only for use by updating
	
	// layer mask split into upper 32 bits (x) and lower 32 bits (y)
	uvec2 layerMask;
	
	// geometry information
	uint firstGeometry;
	uint geometryCount;
	
	// lod information if present
	vec4 lodFactors; // lod factors for lod level 1 to 4
	
	uint highestLod;
	
	// results
	uint cullResult; // result of culling. written by find_content* shaders.
	                 // bit 0-7: flags (like cascade or cube face)
	                 // bit 8: visible (or culled, ecrVisible)
	
	uint lodIndex; // 8-bit: output of calc_lod_level* shaders
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
