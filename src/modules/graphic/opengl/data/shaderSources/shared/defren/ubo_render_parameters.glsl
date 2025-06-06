/*
	<!-- shared/defren/ubo_render_parameters.glsl -->
	<uniformBlock name='RenderParameters' binding='0'/>
*/

UBOLAYOUT_BIND(0) uniform RenderParameters{
	vec4 pAmbient;
	mat4x3 pMatrixV[6];
	mat4 pMatrixP[2];
	mat4 pMatrixVP[6];
	mat3 pMatrixVn[6];
	mat3 pMatrixEnvMap;
	mat4 pMatrixSkyBody[6];
	vec4 pDepthToPosition[2];
	vec2 pDepthToPosition2[2]; // for use by shifted projection matrix
	
	vec2 pDepthTransform; // x=scale, y=offset
	float pEnvMapLodLevel;
	float pNorRoughCorrStrength;
	
	bool pSkinDoesReflections;
	bool pFlipCulling;
	float pClearDepthValue;
	
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
	
	// viewport image coordinates
	// x: minX = 0
	// y: minY = 0
	// z: maxX = renderWidth - 1
	// w: maxY = renderHeight - 1
	ivec4 pViewportImage;
	#define pViewportImageMin (pViewportImage.xy)
	#define pViewportImageMax (pViewportImage.zw)
	
	
	// clip plane if used.
	// xyz: normal
	// w: distance
	vec4 pClipPlane[2]; // normal.xyz, distance
	
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
	vec2 pRenderSize;
	
	// render size for use with compute shaders to skip pixels
	uvec2 pRenderSizeCompute;
	
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
	
	vec4 pDepthOffset[ 4 ]; // x=frontScale, y=frontOffset, z=backScale, w=backOffset
	
	vec3 pParticleLightHack; // temporary hack
	float pBillboardZScale; // billboard z scale if size is fixed to screen
	
	vec3 pFadeRange; // x=fadeNear, y=fadeFar, z=1/(fadeFar-fadeNear)
	
	// transforms from left eye camera space to right eye camera space
	mat4x3 pCameraStereoTransform;
	
	// camera range.
	// x: image distance in meters
	// y: view distance in meters
	vec2 pCameraRange;
	// image distance in meters
	#define pCameraImageDistance (pCameraRange.x)
	// view distance in meters
	#define pCameraViewDistance (pCameraRange.y)
	
	float pCameraAdaptedIntensity;
	
	vec2 pDepthSampleOffset;
	
	
	
	// full screen quad rendering
	
	// transform screen coordinates (-1..1) into texture coordinates (0..pScreenSpaceScale)
	// 
	// vec2 texCoord = inPosition.xy * pFSScreenCoordToTexCoord.xy + pFSScreenCoordToTexCoord.zw
	// 
	// x, z: pScreenSpaceScale.x / 2
	// y, w: pScreenSpaceScale.y / 2
	vec4 pFSScreenCoordToTexCoord;
	
	// transform texture coordinates (0..pScreenSpaceScale) to screen coordinates (-1..1).
	// 
	// vec2 screenCoord = texCoord * pFSTexCoordToScreenCoord.xy + pFSTexCoordToScreenCoord.zw
	// 
	// x: 2 / pScreenSpaceScale.x
	// y: 2 / pScreenSpaceScale.y
	// z, w: -1
	vec4 pFSTexCoordToScreenCoord;
	
	// transform fragment coordinates (0..pRenderSize) into texture coordinates (0..pScreenSpaceScale).
	// 
	// vec2 texCoord = vec2( gl_FragCoord ) * pFSFragCoordToTexCoord.xy + pFSFragCoordToTexCoord.zw
	// 
	// x: pScreenSpaceScale.x / pRenderSize.x
	// y: pScreenSpaceScale.y / pRenderSize.y
	// z: 0.5 / pRenderSize.x
	// w: 0.5 / pRenderSize.y
	vec4 pFSFragCoordToTexCoord;
	
	// transform fragment coordinates (0..pRenderSize) into screen coordinates (-1..1).
	// 
	// vec2 texCoord = vec2( gl_FragCoord ) * pFSFragCoordToScreenCoord.xy + pFSFragCoordToScreenCoord.zw
	// 
	// x: 2 / pRenderSize.x
	// y: 2 / pRenderSize.y
	// z: 1 / pRenderSize.x - 1
	// w: 1 / pRenderSize.y - 1
	vec4 pFSFragCoordToScreenCoord;
	
	
	
	// ssao
	vec4 pSSAOParams1; // self-occlusion, epsilon, scale, randomAngleConstant
	vec4 pSSAOParams2; // count, radius, radius-influence, radius-limit
	vec3 pSSAOParams3; // radiusFactor, mipMapBase, mipMapMaxLevel
	
	
	
	// sssss
	vec4 pSSSSSParams1; // dropSubSurfaceThreshold, tapRadiusFactor, tapRadiusLimit, tapDropRadiusThreshold
	ivec2 pSSSSSParams2; // tapCount, turnCount
	
	
	
	// ssr
	vec4 pSSRParams1; // -1 / edgeSize, 0.5 / edgeSize, powEdge, powRayLen
	#define pSSRCoverageFactor (pSSRParams1.xy)
	#define pSSRPowEdge (pSSRParams1.z)
	#define pSSRPowRayLen (pSSRParams1.w)
	
	vec4 pSSRParams2; // clipReflDirNearDist, roughnessTapCountScale, minMaxTCFactor(xy)
	#define pSSRClipReflDirNearDist (pSSRParams2.x)
	#define pSSRRoughnessTapCountScale (pSSRParams2.y)
	#define pSSRMinMaxTCScale (pSSRParams2.zw)
	
	ivec4 pSSRParams3; // stepCount, subStepCount, maxRayLength, roughnessMaxTaps
	#define pSSRStepCount (pSSRParams3.x)
	#define pSSRSubStepCount (pSSRParams3.y)
	#define pSSRMaxRayLength (pSSRParams3.z)
	#define pSSRRoughnessMapTaps (pSSRParams3.w)
	
	
	
	// lighting
	vec2 pAOSelfShadow; // minShadowIntensity, smoothAngle
	vec2 pLumFragCoordScale;
	
	// global illumination rays
	mat4x3 pGIRayMatrix; // transform from GI space to camera space
	mat3 pGIRayMatrixNormal; // transform from GI space to camera space (requires transpose)
	
	// global illumination
	int pGIHighestCascade; // index of highest cascade
	
	
	
	// tone mapping
	
	// scene key parameters
	// x: exposure
	// y: white scale
	vec2 pToneMapSceneKey;
	#define pToneMapExposure (pToneMapSceneKey.x)
	#define pToneMapWhiteScale (pToneMapSceneKey.y)
	
	// adaption parameters
	// x: lowest luminance
	// y: highest luminance
	// z: adaption time
	vec3 pToneMapAdaption;
	#define pToneMapLowLuminance (pToneMapAdaption.x)
	#define pToneMapHighLuminance (pToneMapAdaption.y)
	#define pToneMapAdaptionTime (pToneMapAdaption.z)
	
	// bloom parameters
	// x: strength
	// y: intensity threshold
	// z: blend
	vec3 pToneMapBloom;
	#define pToneMapBloomStrength (pToneMapBloom.x)
	#define pToneMapBloomIntensity (pToneMapBloom.y)
	#define pToneMapBloomBlend (pToneMapBloom.z)
	
	
	
	// debug
	
	// debug depth scale and shift
	// x: depth scale
	// y: depth shift
	vec2 pDebugDepthTransform;
	#define pDebugDepthScale (pDebugDepthTransform.x)
	#define pDebugDepthOffset (pDebugDepthTransform.y)
	
	
	
	// render conditions, aka specializations
	
	// x: use clip plane
	// y: -
	// z: -
	// w: -
	bvec4 pConditions1;
	#define pCondClipPlane (pConditions1.x)
};

// helper functions
vec2 fsquadScreenCoordToTexCoord( in vec2 screenCoord ){
	return screenCoord * pFSScreenCoordToTexCoord.xy + pFSScreenCoordToTexCoord.zw;
}

vec2 fsquadTexCoordToScreenCoord( in vec2 texCoord ){
	return texCoord * pFSTexCoordToScreenCoord.xy + pFSTexCoordToScreenCoord.zw;
}

vec2 fragCoordToTexCoord( in ivec2 fragCoord ){
	return vec2( fragCoord ) * pFSFragCoordToTexCoord.xy + pFSFragCoordToTexCoord.zw;
}

vec2 fragCoordToScreenCoord( in ivec2 fragCoord ){
	return vec2( fragCoord ) * pFSFragCoordToScreenCoord.xy + pFSFragCoordToScreenCoord.zw;
}
