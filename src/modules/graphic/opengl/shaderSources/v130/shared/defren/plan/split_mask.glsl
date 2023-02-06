

uint helperCalcSplitMask( in int index, in vec3 minExtend, in vec3 maxExtend, in vec2 size, in uint mask ){
	// the last check is used to simulate "i < pSplitCount" since in this case the
	// pSplitSizeThreshold is all 0
	bvec4 cond = bvec4(
		all( lessThanEqual( minExtend, pSplitMaxExtend[ index  ] ) ),
		all( greaterThanEqual( maxExtend, pSplitMinExtend[ index ] ) ),
		all( greaterThan( size, pSplitSizeThreshold[ index ] ) ),
		pSplitSizeThreshold[ index ].x > 0 );
	
	return all( cond ) ? mask : uint( 0 );
}

// calculate split mask
uint calcSplitMask( in vec3 center, in vec3 halfExtend ){
	vec3 minExtend = center - halfExtend;
	vec3 maxExtend = center + halfExtend;
	vec2 size = halfExtend.xy * vec2( 2 );
	
	return helperCalcSplitMask( 0, minExtend, maxExtend, size, uint( 0x1 ) )
		| helperCalcSplitMask( 1, minExtend, maxExtend, size, uint( 0x2 ) )
		| helperCalcSplitMask( 2, minExtend, maxExtend, size, uint( 0x4 ) )
		| helperCalcSplitMask( 3, minExtend, maxExtend, size, uint( 0x8 ) )
		| helperCalcSplitMask( 4, minExtend, maxExtend, size, uint( 0x10 ) );
}
