
bool notIntersectLightFrustum( in vec3 center, in vec3 halfExtend ){
	// if pFrustumPlanes[i] and pFrustumPlanesAbs[i] are equal to 0 for all
	// i >= pFrustumPlaneCount then this calculation is correct without using a for loop
	// to skip calculations for i >= pFrustumPlaneCount. this works since the calculations
	// then becomes "0 - dot(0,x) > dot(0,y)" which boils down to "0 > 0" which is always false
	
	bvec4 result1 = bvec4(
		pFrustumPlanes[ 0 ].w - dot( pFrustumPlanes[ 0 ].xyz, center ) > dot( pFrustumPlanesAbs[ 0 ], halfExtend ),
		pFrustumPlanes[ 1 ].w - dot( pFrustumPlanes[ 1 ].xyz, center ) > dot( pFrustumPlanesAbs[ 1 ], halfExtend ),
		pFrustumPlanes[ 2 ].w - dot( pFrustumPlanes[ 2 ].xyz, center ) > dot( pFrustumPlanesAbs[ 2 ], halfExtend ),
		pFrustumPlanes[ 3 ].w - dot( pFrustumPlanes[ 3 ].xyz, center ) > dot( pFrustumPlanesAbs[ 3 ], halfExtend ) );
	
	bvec2 result2 = bvec2(
		pFrustumPlanes[ 4 ].w - dot( pFrustumPlanes[ 4 ].xyz, center ) > dot( pFrustumPlanesAbs[ 4 ], halfExtend ),
		pFrustumPlanes[ 5 ].w - dot( pFrustumPlanes[ 5 ].xyz, center ) > dot( pFrustumPlanesAbs[ 5 ], halfExtend ) );
	
	return any( result1 ) || any( result2 );
}


bool intersectLightFrustum( in vec3 center, in vec3 halfExtend ){
	// we use the negation of notIntersectBoxShadowHull instead of using modified calculations
	// so we can use the "i >= pFrustumPlaneCount" trick outlined in notIntersectLightFrustum
	
	return ! notIntersectLightFrustum( center, halfExtend );
}
