#define GI_MAX_CASCADES 5

struct sGIParameters{
	mat4x3 matrix; // transform from view space to tracing space
	mat3 matrixNormal; // transform from view space to tracing space (requires transpose)
	ivec3 probeCount; // probe count
	int irradianceMapSize; // occlusion map size in pixels
	ivec3 probeClamp; // maximum probe grid coordinate to clamp to
	int distanceMapSize; // distance map size in pixels
	vec3 probeSpacing; // probe grid spacing
	vec3 probeSpacingInv; // 1 / grid spacing
	float normalBias; // normal bias
	vec3 positionClamp; // clamp position after transformation
	vec2 irradianceMapScale; // occlusion map pixel to texture coordinate scaling
	vec2 distanceMapScale; // distance map pixel to texture coordinate scaling
	ivec3 gridCoordShift; // grid coordinate shift (wrapping around)
	float irradianceGamma;
	float selfShadowBias;
	vec3 gridOrigin;
	ivec3 gridCoordUnshift;
};

UBOLAYOUT uniform GIParameters{
	sGIParameters pGIParams[ GI_MAX_CASCADES ];
};
