UBOLAYOUT_BIND(1) uniform GIParameters{
	vec2 pGISampleImageScale;
	int pGIProbeCount;
	int pGIRaysPerProbe;
	
	ivec3 pGIGridProbeCount;
	int pGIProbesPerLine;
	
	vec3 pGIGridProbeSpacing;
	int pGIBVHInstanceRootNode; // index to root node or -1 if BVH is empty
	
	vec2 pGIIrradianceMapScale;
	vec2 pGIDistanceMapScale;
	
	int pGIIrradianceMapSize;
	int pGIDistanceMapSize;
	float pGIMaxProbeDistance;
	float pGIDepthSharpness;
	
	vec3 pGIGridOrigin;
	ivec3 pGIGridCoordUnshift;
	
	vec3 pGIFieldSize;
	float pGIBlendUpdateProbe;
	
	vec3 pGIMoveMaxOffset;
	float pGIMoveMinDistToSurface;
	
	vec2 pGIRayMapScale;
	int pGIMaterialMapsPerRow;
	int pGIMaterialMapSize;
	
	float pGIIrradianceGamma;
	float pGIInvIrradianceGamma;
	float pGISelfShadowBias;
	int pGICascade;
	
	vec3 pGIDetectionBox;
	int pRayCacheProbeCount;
	
	vec3 pGIBVHOffset;
};

UBOLAYOUT_BIND(3) uniform GIProbePositions{
	vec4 pGIProbePosition[GIMaxCountProbePositions]; // position(xyz), flags(w)
};

UBOLAYOUT_BIND(4) uniform GIRayDirections{
	vec3 pGIRayDirection[GIMaxCountRayDirections];
};
