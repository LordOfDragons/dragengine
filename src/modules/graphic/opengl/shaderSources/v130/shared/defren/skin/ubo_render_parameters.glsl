UBOLAYOUT uniform RenderParameters{
	#ifdef GS_RENDER_CUBE
		vec4 pAmbient;
		mat4x3 pCubeMatrixV[ 6 ];
		mat4 pMatrixP;
		mat4 pCubeMatrixVP[ 6 ];
		mat3 pCubeMatrixVn[ 6 ];
		mat3 pMatrixEnvMap;
		
		#define pMatrixV pCubeMatrixV[ 0 ]
		#define pMatrixVP pCubeMatrixVP[ 0 ]
		#define pMatrixVn pCubeMatrixVn[ 0 ]
		
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
	vec4 pDepthOffset; // x=frontScale, y=frontOffset, z=backScale, w=backOffset
	vec3 pParticleLightHack; // temporary hack
	vec3 pFadeRange; // x=fadeNear, y=farFar, z=1/(fadeFar-fadeNear)
	float pBillboardZScale; // billboard z scale if size is fixed to screen
};
