// helper methods for cube rendering with geometry shader

/**
 * Determine if primitive is inside frustum.
 * 
 * Only does most important tests to be fast. This implies the test is conservative and thus
 * some primitives are considered inside although with a precise test they would be outside.
 * 
 * To avoid doing more calculations than necessary we operate on homogenous coordinates.
 * For those the basic rule is:
 *   -w < x < w
 *   -w < y < w
 *   -w < z < w
 * 
 * The input points are homogenous clip coordinates without normalization.
 * 
 * Returns true if primitive is inside frustum. This result can be a false positive.
 * Returns false if primitive is outside frustum. This result is always exact.
 */
bool cubeInsideFrustum( in vec4 p1, in vec4 p2, in vec4 p3 ){
	// test z-near clipping plane
	if( p1.z >= p1.w && p2.z >= p2.w && p3.z >= p3.w ){
		return false;
	}
	
	// test z-far clipping plane
	if( p1.z <= -p1.w && p2.z <= -p2.w && p3.z <= -p3.w ){
		return false;
	}
	
	// test right clipping plane
	if( p1.x >= p1.w && p2.x >= p2.w && p3.x >= p3.w ){
		return false;
	}
	
	// test left clipping plane
	if( p1.x <= -p1.w && p2.x <= -p2.w && p3.x <= -p3.w ){
		return false;
	}
	
	// test top clipping plane
	if( p1.y >= p1.w && p2.y >= p2.w && p3.y >= p3.w ){
		return false;
	}
	
	// test bottom clipping plane
	if( p1.y <= -p1.w && p2.y <= -p2.w && p3.y <= -p3.w ){
		return false;
	}
	
	// corner tests are skipped. they are too expensive and result in barely any
	// triangles to be skipped.
	
	// primitive is likely inside
	return true;
}
