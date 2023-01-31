
// world octree data flags
const uint wodfComponent = uint( 0x1 );
const uint wodfBillboard = uint( 0x2 );
const uint wodfParticleEmitter = uint( 0x4 );
const uint wodfLight = uint( 0x8 );
const uint wodfStatic = uint( 0x10 );


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
