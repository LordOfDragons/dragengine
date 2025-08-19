const int GIMaxCountProbePositions = 2048;
const int GIMaxCountRayDirections = 256;

// 0: [0]+=[1],  [2]+=[3],   ...,        [62]+=[63]
// 1: [0]+=[2],  [4]+=[6],   ...,        [60]+=[62]
// 2: [0]+=[4],  [8]+=[12],  ...,        [56]+=[60]
// 3: [0]+=[8],  [16]+=[24], [32]+=[40], [48]+=[56]
// 4: [0]+=[16], [32]+=[48]
// 5: [0]+=[32]

const uvec3 combineParams64Mul[5] = uvec3[5](
	uvec3( 0, 2, 2 ), uvec3( 0, 4, 4 ), uvec3( 0, 8, 8 ), uvec3( 0, 16, 16 ), uvec3( 0, 32, 32 ) );

const uvec3 combineParams64Add[5] = uvec3[5](
	uvec3( 32, 0, 1 ), uvec3( 16, 0, 2 ), uvec3( 8, 0, 4 ), uvec3( 4, 0, 8 ), uvec3( 2, 0, 16 ) );

const int combineParams64Count = 5;
const uvec3 combineParams64Last = uvec3( 1, 0, 32 );
