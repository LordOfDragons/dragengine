
// test lod levels starting with the smallest one for the first lod level which
// still yields an error on screen not larger than the maximum pixel error.
// 
// z [m]: object distance minus radius
// le(i) [m]: max error at lod i
// se: screen max error ratio (maxError[px] / screenWidth[px])
// 
// hw [m] = z [m] * tan(fov): half width of screen at distance z
// e [m] = hw [m] * se: max error at distance z
//       = z [m] * tan(fov) * se
// 
// FindConfig.pLodFactor has to be calculated like this:
//   const float factorX = tanf( fovX * 0.5f ) * pMaxPixelError / screenWidth;
//   const float factorY = tanf( fovY * 0.5f ) * pMaxPixelError / screenHeight;
//   const float factor = decMath::min( factorX, factorY );
uint calcLodProjection( in uint index ){
	uint lodLevel = uint( 0 );
	
	// calculate a sphere around the element extends. this causes the lod calculation
	// to be very cautious but this can be compensated with better calculation later
	// on for elements with elongated box shapes
	vec3 minExtend = pElement[ index ].minExtend;
	vec3 maxExtend = pElement[ index ].maxExtend;
	vec3 center = ( minExtend + maxExtend ) * vec3( 0.5 );
	float radius = length( maxExtend - minExtend ) * 0.5;
	float elementDistance = dot( center - pCameraPosition, pCameraView ) - radius;
	
	if( elementDistance > 1 ){
		float maxError = pLodFactor * elementDistance;
		
		#ifdef GLSL_450
		uvec4 lod = mix( uvec4( 0 ), uvec4( 1, 2, 3, 4 ),
			lessThanEqual( pElement[ index ].lodFactors, vec4( maxError ) ) );
		#else
		uvec4 lod = uvec4(
			pElement[ index ].lodFactors.x <= maxError ? uint( 1 ) : uint( 0 ),
			pElement[ index ].lodFactors.y <= maxError ? uint( 1 ) : uint( 0 ),
			pElement[ index ].lodFactors.z <= maxError ? uint( 1 ) : uint( 0 ),
			pElement[ index ].lodFactors.w <= maxError ? uint( 1 ) : uint( 0 ) );
		#endif
		
		lod.xy = max( lod.xy, lod.zw );
		lodLevel = max( lod.x, lod.y );
	}
	
	return min( lodLevel + pLodOffset, pElement[ index ].highestLod );
}


// calculate lod using selected method
uint calcLod( in uint index ){
	if( pLodMethod == lmHighest ){
		return pElement[ index ].highestLod;
		
	}else if( pLodMethod == lmProjection ){
		return calcLodProjection( index );
		
	}else if( pLodMethod == lmOrthogonal ){
		return uint( 0 );
		
	}else{ // lowest
		return uint( 0 );
	}
}
