
struct sCounter {
	uvec3 workGroupSize;
	uint counter;
};


// deoglRenderCompute counters
////////////////////////////////

const uint pRenderComputeCounterCount = uint( 2 );

// enum eCounters
const uint erccVisibleGeometries = uint( 0 );
const uint erccRenderTaskSubInstanceGroups = uint( 1 );
