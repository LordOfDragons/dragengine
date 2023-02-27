
UBOLAYOUT uniform FindConfig{
	uint pNodeCount;
	uint pElementCount;
	
	// camera frustum
	vec4 pFrustumPlanes[ 6 ]; // xyz=normal, w=distance
	vec3 pFrustumPlanesAbs[ 6 ];
	bvec3 pFrustumSelect[ 6 ]; // greaterThan(pFrustumPlanes[x], vec3(0))
	
	// GI cascade box
	vec3 pGIMinExtend;
	vec3 pGIMaxExtend;
	
	// element culling
	uvec2 pLayerMask;  // x=higher 32 bits, y=lower 32 bits
	bool pCullLayerMask;
	uint pCullFlags;
	
	vec3 pCameraPosition;  // pPlan.GetCameraPosition()
	vec3 pCameraView;  // pPlan.GetInverseCameraMatrix().TransformView()
	float pErrorScaling;  // deoglRPVisitorFindContent
	
	// hull
	vec4 pHullEdgeNormal[ 5 ]; // xy=normal, zw=normalAbs
	float pHullEdgeDistance[ 5 ];
	
	// light space
	mat4x3 pMatrixLightSpace;
	
	// shadow axis
	vec3 pShadowAxis[ 3 ];
	float pLightShaftFar;
	
	// cascaded sky light shadow
	vec3 pSplitMinExtend[ 4 ];
	vec3 pSplitMaxExtend[ 4 ];
	vec2 pSplitSizeThreshold[ 4 ];
	int pSplitCount;
	
	// lod calculation
	float pLodFactor;
	float pLodMaxPixelError;
	uint pLodOffset;
	uint pLodMethod; // values from log method (lm*)
};
