
// calculate split mask
uint calcSplitMask( in vec3 center, in vec3 halfExtend ){
	vec3 minExtend = center - halfExtend;
	vec3 maxExtend = center + halfExtend;
	vec2 size = halfExtend.xy * vec2( 2 );
	uint mask = uint( 0 );
	bvec3 cond;
	int i;
	
	for( i=0; i<pSplitCount; i++ ){
		cond.x = all( lessThanEqual( minExtend, pSplitMaxExtend[ i  ] ) );
		cond.y = all( greaterThanEqual( maxExtend, pSplitMinExtend[ i ] ) );
		cond.z = all( greaterThan( size, pSplitSizeThreshold[ i ] ) );
		
		mask |= all( cond ) ? uint( 1 ) << i : uint( 0 );
	}
	
	return mask;
}
