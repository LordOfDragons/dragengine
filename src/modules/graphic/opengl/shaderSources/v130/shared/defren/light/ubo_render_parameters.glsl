UBOLAYOUT uniform RenderParameters{
	vec4 pPosTransform;
	vec2 pAOSelfShadow; // minShadowIntensity, smoothAngle
	vec2 pLumFragCoordScale;
	
	// occlusion tracing
	mat4x3 pOTMatrix; // transform from view space to tracing space
	mat3 pOTMatrixNormal; // transform from view space to tracing space
	ivec3 pOTProbeCount; // probe count
	int pOTOcclusionMapSize;
	ivec3 pOTProbeClamp; // maximum probe index to clamp to
	int pOTDistanceMapSize;
	vec3 pOTProbeSpacing;
	vec3 pOTProbeSpacingInv;
	vec3 pOTProbeOrigin;
	float pOTNormalBias;
	vec2 pOTOcclusionMapScale;
	vec2 pOTDistanceMapScale;
	float pOTEnergyPreservation;
};
