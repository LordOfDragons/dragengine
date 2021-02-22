UBOLAYOUT uniform TracingParameters{
	vec2 pSampleImageScale;
	int pProbeCount;
	int pRaysPerProbe;
	
	ivec3 pGridProbeCount;
	int pProbesPerLine;
	
	vec3 pGridProbeSpacing;
	int pBVHInstanceRootNode; // index to root node or -1 if BVH is empty
	
	vec2 pOcclusionMapScale;
	vec2 pDistanceMapScale;
	
	int pOcclusionMapSize;
	int pDistanceMapSize;
	float pMaxProbeDistance;
	float pDepthSharpness;
	
	vec3 pFieldOrigin;
	
	ivec4 pProbeIndex[ MAX_PROBE_INDEX_COUNT ];
	
	vec4 pProbePosition[ MAX_PROBE_UPDATE_COUNT ]; // position(xyz), blendFactor(w)
	
	vec3 pRayDirection[ MAX_RAYS_PER_PROBE ];
};
