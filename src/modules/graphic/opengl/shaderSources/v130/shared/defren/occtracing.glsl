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
	ivec4 pProbeIndex[ 64 ]; // max probe update count / 4
	vec3 pProbePosition[ 256 ]; // max probe update count
	vec3 pRayDirection[ 64 ]; // ray count
};
