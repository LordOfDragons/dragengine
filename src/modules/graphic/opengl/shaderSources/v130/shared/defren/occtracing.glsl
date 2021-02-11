UBOLAYOUT uniform TracingParameters{
	vec2 pSampleImageScale;
	int pProbeCount;
	int pRaysPerProbe;
	int pProbesPerLine;
	int pBVHInstanceRootNode;
	ivec3 pGridProbeCount;
	vec3 pGridProbeSpacing;
	int pOcclusionMapSize;
	int pDistanceMapSize;
	vec2 pOcclusionMapScale;
	vec2 pDistanceMapScale;
	float pMaxProbeDistance;
	float pDepthSharpness;
	float pHysteresis;
	ivec4 pProbeIndex[ PROBE_INDEX_COUNT ];
	vec3 pProbePosition[ MAX_PROBE_UPDATE_COUNT ];
	vec3 pRayDirection[ RAYS_PER_PROBE ];
};
