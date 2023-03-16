
// test if the box projected to the shadow map plane hits the frustum hull projected to the shadow map plane
bool notIntersectHull( in vec2 center, in vec2 halfExtend ){
	// if pHullEdgeNormal[i], pHullEdgeNormalAbs[i] and pHullEdgeDistance[i]
	// are equal to 0 for all i >= pHullEdgeCount then this calculation is correct without
	// using a for loop to skip calculations for i >= pHullEdgeCount. this works since
	// the calculations then becomes "dot(0,x) > dot(0,y) + 0" which boils down to "0 > 0"
	// which is always false
	
	bvec4 result1 = bvec4(
		dot( pHullEdgeNormal[ 0 ].xy, center ) > dot( pHullEdgeNormal[ 0 ].zw, halfExtend ) + pHullEdgeDistance[ 0 ],
		dot( pHullEdgeNormal[ 1 ].xy, center ) > dot( pHullEdgeNormal[ 1 ].zw, halfExtend ) + pHullEdgeDistance[ 1 ],
		dot( pHullEdgeNormal[ 2 ].xy, center ) > dot( pHullEdgeNormal[ 2 ].zw, halfExtend ) + pHullEdgeDistance[ 2 ],
		dot( pHullEdgeNormal[ 3 ].xy, center ) > dot( pHullEdgeNormal[ 3 ].zw, halfExtend ) + pHullEdgeDistance[ 3 ] );
	
	bool result2 =
		dot( pHullEdgeNormal[ 4 ].xy, center ) > dot( pHullEdgeNormal[ 4 ].zw, halfExtend ) + pHullEdgeDistance[ 4 ];
	
	return any( result1 ) || result2;
}


bool intersectHull( in vec2 center, in vec2 halfExtend ){
	// we use the negation of notIntersectBoxShadowHull instead of using modified calculations
	// so we can use the "i >= pHullEdgeCount" trick outlined in notIntersectHull
	
	return ! notIntersectHull( center, halfExtend );
}

