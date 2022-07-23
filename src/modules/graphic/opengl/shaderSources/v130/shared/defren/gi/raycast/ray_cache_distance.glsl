uniform mediump sampler2DArray texGIRayCastDistanceLimit;

// get ray cache distance for ray
float giRayCastCacheDistance( in int instanceID, in int rayIndex, in int layer ){
	// calculate probe index from instance ID. this is the real probe index
	int probeIndex = pGIProbeIndex[ instanceID >> 2 ][ instanceID & 3 ]; // 4 IDs per array entry
	
	// calculatee texture coordinate to sample. probes are packed one after the other
	// starting at the bottom plane going all the way up
	ivec3 tc = ivec3( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ), probeIndex / pGIProbesPerLine, layer );
	
	// offset texture coordinate by ray to query
	tc.x += rayIndex;
	
	// texture is R16F
	return texelFetch( texGIRayCastDistanceLimit, tc, 0 ).r;
}
