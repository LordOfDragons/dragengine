UBOLAYOUT uniform GIParameters{
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
	
	vec3 pGIFieldOrigin;
	float pGIBlendUpdateProbe;
	
	vec3 pGIMoveMaxOffset;
	float pGIMoveMinDistToSurface;
	
	vec2 pGIRayMapScale;
	int pGIMaterialMapsPerRow;
	int pGIMaterialMapSize;
};

UBOLAYOUT uniform GIProbeIndices{
	ivec4 pGIProbeIndex[ 1024 ]; // 4 indices per element
};

UBOLAYOUT uniform GIProbePositions{
	vec4 pGIProbePosition[ 4096 ]; // position(xyz), blendFactor(w)
};

UBOLAYOUT uniform GIRayDirections{
	vec3 pGIRayDirection[ 512 ];
};
