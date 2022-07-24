UBOLAYOUT uniform RenderParameters{
	#ifdef GS_RENDER_STEREO
		vec4 pDepthToPosition[ 2 ];
		vec2 pDepthToPosition2[ 2 ];
	#else
		vec4 pDepthToPosition;
		vec2 pDepthToPosition2;
	#endif
	
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
