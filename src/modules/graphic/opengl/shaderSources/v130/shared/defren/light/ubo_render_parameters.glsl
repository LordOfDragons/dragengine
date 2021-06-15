UBOLAYOUT uniform RenderParameters{
	vec4 pPosTransform;
	vec2 pDepthSampleOffset;
	vec2 pAOSelfShadow; // minShadowIntensity, smoothAngle
	vec2 pLumFragCoordScale;
	
	// global illumination rays
	mat4x3 pGIRayMatrix; // transform from GI space to camera space
	mat3 pGIRayMatrixNormal; // transform from GI space to camera space (requires transpose)
	mat4 pGICameraProjection;
	
	// global illumination
	int pGIHighestCascade; // index of highest cascade
};
