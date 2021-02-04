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
	vec3 pOTProbeSpacingInv; // 1 / grid spacing
	vec3 pOTProbeOrigin; // position of (0,0,0) probe coordinate relative to center
	float pOTNormalBias; // normal bias
	vec2 pOTOcclusionMapScale; // occlusion map pixel to texture coordinate scaling
	vec2 pOTDistanceMapScale; // distance map pixel to texture coordinate scaling
	float pOTEnergyPreservation; // energy preservation factor
};
