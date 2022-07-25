UBOLAYOUT uniform RenderParameters{
	#ifdef GS_RENDER_CUBE
		vec4 pAmbient;
		mat4x3 pMatrixV[ 6 ];
		mat4 pMatrixP;
		mat4 pMatrixVP[ 6 ];
		mat3 pMatrixVn[ 6 ];
		mat3 pMatrixEnvMap;
		vec4 pDepthToPosition;
		vec2 pDepthToPosition2; // for use by shifted projection matrix
		
		#define pMatrixPDefault pMatrixP
		
	#elif defined GS_RENDER_CASCADED
		vec4 pAmbient;
		mat4x3 pMatrixV[ 4 ];
		mat4 pMatrixP; // identify
		mat4 pMatrixVP[ 4 ]; // same as pCascadeMatrixV
		mat3 pMatrixVn[ 4 ];
		mat3 pMatrixEnvMap;
		vec4 pDepthToPosition;
		vec2 pDepthToPosition2; // for use by shifted projection matrix
		
		#define pMatrixPDefault pMatrixP
		
	#elif defined GS_RENDER_STEREO
		vec4 pAmbient;
		mat4x3 pMatrixV[ 2 ];
		mat4 pMatrixP[ 2 ];
		mat4 pMatrixVP[ 2 ];
		mat3 pMatrixVn[ 2 ];
		mat3 pMatrixEnvMap;
		vec4 pDepthToPosition[ 2 ];
		vec2 pDepthToPosition2[ 2 ]; // for use by shifted projection matrix
		
		#define pMatrixPDefault pMatrixP[ 0 ]
		
	#else
		vec4 pAmbient;
		mat4x3 pMatrixV;
		mat4 pMatrixP;
		mat4 pMatrixVP;
		mat3 pMatrixVn;
		mat3 pMatrixEnvMap;
		vec4 pDepthToPosition;
		vec2 pDepthToPosition2; // for use by shifted projection matrix
		
		#define pMatrixPDefault pMatrixP
	#endif
	
	vec2 pDepthTransform; // x=scale, y=offset
	float pEnvMapLodLevel;
	float pNorRoughCorrStrength;
	
	bool pSkinDoesReflections;
	bool pFlipCulling;
	
	// normalize viewport texture coordinates.
	// x: minX = 0
	// y: minY = 0
	// z: maxX = (renderWidth - 1) / realTextureWidth
	// w: maxY = (renderHeight - 1) / realTextureHeight
	vec4 pViewport;
	// (minX, minY) = (0, 0)
	#define pViewportMin (pViewport.xy)
	// (maxX, MinY) = ((renderWidth - 1) / realTextureWidth, (renderHeight - 1) / realTextureHeight)
	#define pViewportMax (pViewport.zw)
	
	// clip plane if used.
	// xyz: normal
	// w: distance
	vec4 pClipPlane;
	
	// screen space conversion parameters
	// x: scaleU = renderWidth / realTextureWidth
	// y: scaleV = renderHeight / realTextureHeight
	// z: pixelSizeU = 1 / realTextureWidth
	// w: pixelSizeV = 1 / realTextureHeight
	vec4 pScreenSpace;
	// (scaleU, scaleV) = (renderWidth / realTextureWidth, renderHeight / realTextureHeight)
	#define pScreenSpaceScale (pScreenSpace.xy)
	// (pixelSizeU, pixelSizeV) = (1 / realTextureWidth, 1 / realTextureHeight)
	#define pScreenSpacePixelSize (pScreenSpace.zw)
	// pixelSizeU = 1 / realTextureWidth
	#define pScreenSpacePixelSizeU (pScreenSpace.z)
	// pixelSizeV = 1 / realTextureHeight
	#define pScreenSpacePixelSizeV (pScreenSpace.w)
	
	// render size.
	// x: width (pixels)
	// y: height (pixels)
	// z: layerCount
	vec3 pRenderSize;
	// layerCount
	#define pRenderLayerCount (pRenderSize.z)
	
	// mip map parameters.
	// x: pixelSizeU (size of pixel of mip map level 0 in meters)
	// y: pixelSizeV (size of pixel of mip map level 0 in meters)
	// z: maxLevel (level index of highest mip map)
	// w: maxScale (scale factor from lowest to highest mip map level) = pow(2, maxLevel)
	vec4 pMipMapParams;
	// (pixelSizeU, pixelSizeV)
	#define pMipMapPixelSize (pMipMapParams.xy)
	// pixelSizeU
	#define pMipMapPixelSizeU (pMipMapParams.x)
	// pixelSizeV
	#define pMipMapPixelSizeV (pMipMapParams.y)
	// maxLevel
	#define pMipMapMaxLevel (pMipMapParams.z)
	// maxScale = pow(2, maxLevel)
	#define pMipMapMaxScale (pMipMapParams.w)
	
	#ifdef GS_RENDER_CASCADED
		vec4 pDepthOffset[ 4 ]; // x=frontScale, y=frontOffset, z=backScale, w=backOffset
	#else
		vec4 pDepthOffset; // x=frontScale, y=frontOffset, z=backScale, w=backOffset
	#endif
	
	vec3 pParticleLightHack; // temporary hack
	float pBillboardZScale; // billboard z scale if size is fixed to screen
	
	vec3 pFadeRange; // x=fadeNear, y=fadeFar, z=1/(fadeFar-fadeNear)
	float pCameraAdaptedIntensity;
	
	// camera range.
	// x: image distance in meters
	// y: view distance in meters
	vec2 pCameraRange;
	// image distance in meters
	#define pCameraImageDistance (pCameraRange.x)
	// view distance in meters
	#define pCameraViewDistance (pCameraRange.y)
	
	vec2 pDepthSampleOffset;
	
	// full screen quad rendering
	vec4 pFSQuadTransform; // vertex shader transform position
	vec4 pFSQuadTCTransform; // fragment shader transform texture coordinates
	vec4 pFSQuadTCClamp; // fragment shader clamp texture coordinates
	
	// ssao
	vec4 pSSAOParams1; // self-occlusion, epsilon, scale, randomAngleConstant
	vec4 pSSAOParams2; // count, radius, radius-influence, radius-limit
	vec3 pSSAOParams3; // radiusFactor, mipMapBase, mipMapMaxLevel
	
	// sssss
	vec4 pSSSSSParams1; // dropSubSurfaceThreshold, tapRadiusFactor, tapRadiusLimit, tapDropRadiusThreshold
	ivec2 pSSSSSParams2; // tapCount, turnCount
	
	// lighting
	vec2 pAOSelfShadow; // minShadowIntensity, smoothAngle
	vec2 pLumFragCoordScale;
	
	// global illumination rays
	mat4x3 pGIRayMatrix; // transform from GI space to camera space
	mat3 pGIRayMatrixNormal; // transform from GI space to camera space (requires transpose)
	
	// global illumination
	int pGIHighestCascade; // index of highest cascade
};
