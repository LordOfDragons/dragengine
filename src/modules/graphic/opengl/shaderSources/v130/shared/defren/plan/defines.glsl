
// configuration
UBOLAYOUT uniform FindConfig{
	// node culling
	uint pNodeCount;
	
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
struct sWorldOctreeNode{
	vec3 minExtend;
	vec3 maxExtend;
	
	uint firstNode;
	uint childNodeCount;
	uint elementCount;
};

UBOLAYOUT_BIND(0) readonly buffer WorldOctreeNode {
	sWorldOctreeNode pWorldOctreeNode[];
};


// compute shader world octree element
struct sWorldOctreeElement{
	vec3 minExtend;
	vec3 maxExtend;
	uint elementindex;
	uint flags;
	
	// layer mask split into upper 32 bits (x) and lower 32 bits (y)
	uvec2 layerMask;
};

UBOLAYOUT_BIND(1) readonly buffer WorldOctreeElement {
	sWorldOctreeElement pWorldOctreeElement[];
};
