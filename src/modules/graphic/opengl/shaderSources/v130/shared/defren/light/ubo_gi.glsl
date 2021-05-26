UBOLAYOUT uniform GIParameters{
	mat4x3 pGIMatrix; // transform from view space to tracing space
	mat3 pGIMatrixNormal; // transform from view space to tracing space (requires transpose)
	ivec3 pGIProbeCount; // probe count
	int pGIIrradianceMapSize; // occlusion map size in pixels
	ivec3 pGIProbeClamp; // maximum probe grid coordinate to clamp to
	int pGIDistanceMapSize; // distance map size in pixels
	vec3 pGIProbeSpacing; // probe grid spacing
	vec3 pGIProbeSpacingInv; // 1 / grid spacing
	float pGINormalBias; // normal bias
	vec3 pGIPositionClamp; // clamp position after transformation
	float pGIEnergyPreservation; // energy preservation factor
	vec2 pGIIrradianceMapScale; // occlusion map pixel to texture coordinate scaling
	vec2 pGIDistanceMapScale; // distance map pixel to texture coordinate scaling
	ivec3 pGIGridCoordShift; // grid coordinate shift (wrapping around)
	float pGIIrradianceGamma;
};
