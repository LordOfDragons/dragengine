UBOLAYOUT uniform RenderParameters{
	#ifdef GS_RENDER_CUBE
		vec4 pAmbient;
		mat4x3 pMatrixV[ 6 ];
		mat4 pMatrixP;
		mat4 pMatrixVP[ 6 ];
		mat3 pMatrixVn[ 6 ];
		mat3 pMatrixEnvMap;
		
	#elif defined GS_RENDER_CASCADED
		vec4 pAmbient;
		mat4x3 pMatrixV[ 4 ];
		mat4 pMatrixP; // identify
		mat4 pMatrixVP[ 4 ]; // same as pCascadeMatrixV
		mat3 pMatrixVn[ 4 ];
		mat3 pMatrixEnvMap;
		
	#elif defined GS_RENDER_STEREO
		vec4 pAmbient;
		mat4x3 pMatrixV[ 2 ];
		mat4 pMatrixP[ 2 ];
		mat4 pMatrixVP[ 2 ];
		mat3 pMatrixVn[ 2 ];
		mat3 pMatrixEnvMap;
		
	#else
		vec4 pAmbient;
		mat4x3 pMatrixV;
		mat4 pMatrixP;
		mat4 pMatrixVP;
		mat3 pMatrixVn;
		mat3 pMatrixEnvMap;
	#endif
	
	vec2 pDepthTransform; // x=scale, y=offset
	float pEnvMapLodLevel;
	float pNorRoughCorrStrength;
	
	bool pSkinDoesReflections;
	bool pFlipCulling;
	
	vec4 pViewport; // minX, minY, maxX, maxY
	vec4 pClipPlane; // normal.xyz, distance
	vec4 pScreenSpace; // x=scaleU, y=scaleV, z=pixelSizeU, w=pixelSizeV
	
	#ifdef GS_RENDER_CASCADED
		vec4 pDepthOffset[ 4 ]; // x=frontScale, y=frontOffset, z=backScale, w=backOffset
	#else
		vec4 pDepthOffset; // x=frontScale, y=frontOffset, z=backScale, w=backOffset
	#endif
	
	vec3 pParticleLightHack; // temporary hack
	vec3 pFadeRange; // x=fadeNear, y=farFar, z=1/(fadeFar-fadeNear)
	float pBillboardZScale; // billboard z scale if size is fixed to screen
	float pCameraAdaptedIntensity;
};
