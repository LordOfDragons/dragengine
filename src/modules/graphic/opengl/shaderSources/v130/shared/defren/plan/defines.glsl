
// configuration
UBOLAYOUT uniform FindConfig{
	uint pNodeCount;
	uint pElementCount;
	uint pUpdateElementCount;
	
	vec4 pFrustumPlanes[ 6 ]; // xyz=normal, w=distance
	bvec3 pFrustumSelect[ 6 ]; // greaterThan(pFrustumPlanes[x], vec3(0))
	
	vec3 pGIMinExtend;
	vec3 pGIMaxExtend;
	
	// element culling
	uvec2 pLayerMask;  // x=higher 32 bits, y=lower 32 bits
	bool pCullLayerMask;
	uint pCullFlags;
	
	vec3 pCameraPosition;  // pPlan.GetCameraPosition()
	vec3 pCameraView;  // pPlan.GetInverseCameraMatrix().TransformView()
	float pErrorScaling;  // deoglRPVisitorFindContent
};


// world octree data flags
const uint wodfComponent = uint( 0x1 );
const uint wodfComponentDynamic = uint( 0x2 );
const uint wodfBillboard = uint( 0x4 );
const uint wodfParticleEmitter = uint( 0x8 );
const uint wodfLight = uint( 0x10 );


// compute shader world octree node
#if 0
struct sWorldOctreeNode{
	vec3 minExtend;
	uint firstNode;
	
	vec3 maxExtend;
	uint childNodeCount;
	
	uint firstElement;
	uint elementCount;
};
#endif


// compute shader element
struct sElement{
	vec3 minExtend;
	uint flags;
	
	vec3 maxExtend;
	uint updateIndex; // only for use by updating
	
	// layer mask split into upper 32 bits (x) and lower 32 bits (y)
	uvec2 layerMask;
};
