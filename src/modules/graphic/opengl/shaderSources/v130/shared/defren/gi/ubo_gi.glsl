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
	
	int pGIMaterialMapsPerRow;
	int pGIMaterialMapSize;
	
	vec3 pGIMoveMaxOffset;
	float pGIMoveMinDistToSurface;
	
	ivec4 pGIProbeIndex[ GI_PROBE_INDEX_COUNT ];
	
	vec4 pGIProbePosition[ GI_PROBE_COUNT ]; // position(xyz), blendFactor(w)
	
	vec3 pGIRayDirection[ GI_RAYS_PER_PROBE ];
};
