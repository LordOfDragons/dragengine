
bool intersectGI( in vec3 minExtend, in vec3 maxExtend ){
	return all( greaterThanEqual( pGIMaxExtend, minExtend ) )
		&& all( lessThanEqual( pGIMinExtend, maxExtend ) );
}

bool notIntersectGI( in vec3 minExtend, in vec3 maxExtend ){
	return any( lessThan( pGIMaxExtend, minExtend ) )
		|| any( greaterThan( pGIMinExtend, maxExtend ) );
}
