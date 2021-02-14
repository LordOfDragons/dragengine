UBOLAYOUT uniform RenderParameters{
	vec4 pPosTransform;
	vec2 pAOSelfShadow; // minShadowIntensity, smoothAngle
	vec2 pLumFragCoordScale;
	
	// occlusion tracing
	mat4x3 pOTMatrix; // transform from view space to tracing space
	mat3 pOTMatrixNormal; // transform from view space to tracing space
	ivec3 pOTProbeCount; // probe count
	int pOTOcclusionMapSize; // occlusion map size in pixels
	ivec3 pOTProbeClamp; // maximum probe grid coordinate to clamp to
	int pOTDistanceMapSize; // distance map size in pixels
	vec3 pOTProbeSpacing; // probe grid spacing
	bool pOTEnabled;
	vec3 pOTProbeSpacingInv; // 1 / grid spacing
	float pOTNormalBias; // normal bias
	vec3 pOTProbeOrigin; // position of (0,0,0) probe coordinate relative to center
	float pOTEnergyPreservation; // energy preservation factor
	vec2 pOTOcclusionMapScale; // occlusion map pixel to texture coordinate scaling
	vec2 pOTDistanceMapScale; // distance map pixel to texture coordinate scaling
	ivec3 pOTGridCoordShift; // grid coordinate shift (wrapping around)
};
