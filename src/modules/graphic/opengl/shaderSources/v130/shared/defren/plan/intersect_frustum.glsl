
uniform vec4 pFrustumPlanes[ 6 ]; // xyz=normal, w=distance
uniform bvec3 pFrustumSelect[ 6 ]; // greaterThan(pFrustumPlanes[x], vec3(0))


bool intersectFrustum( in vec3 minExtend, in vec3 maxExtend ){
	bvec4 result1 = bvec4(
		pFrustumPlanes[ 0 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 0 ] ) >= pFrustumPlanes[ 0 ].w,
		pFrustumPlanes[ 1 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 1 ] ) >= pFrustumPlanes[ 1 ].w,
		pFrustumPlanes[ 2 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 2 ] ) >= pFrustumPlanes[ 2 ].w,
		pFrustumPlanes[ 3 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 3 ] ) >= pFrustumPlanes[ 3 ].w );
	
	bvec2 result2 = bvec2(
		pFrustumPlanes[ 4 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 4 ] ) >= pFrustumPlanes[ 4 ].w,
		pFrustumPlanes[ 5 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 5 ] ) >= pFrustumPlanes[ 5 ].w );
	
	return all( result1 ) && all( result2 );
}


bool notIntersectFrustum( in vec3 minExtend, in vec3 maxExtend ){
	bvec4 result1 = bvec4(
		pFrustumPlanes[ 0 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 0 ] ) < pFrustumPlanes[ 0 ].w,
		pFrustumPlanes[ 1 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 1 ] ) < pFrustumPlanes[ 1 ].w,
		pFrustumPlanes[ 2 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 2 ] ) < pFrustumPlanes[ 2 ].w,
		pFrustumPlanes[ 3 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 3 ] ) < pFrustumPlanes[ 3 ].w );
	
	bvec2 result2 = bvec2(
		pFrustumPlanes[ 4 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 4 ] ) < pFrustumPlanes[ 4 ].w,
		pFrustumPlanes[ 5 ].xyz * mix( minExtend, maxExtend, pFrustumSelect[ 5 ] ) < pFrustumPlanes[ 5 ].w );
	
	return any( result1 ) || any( result2 );
}
