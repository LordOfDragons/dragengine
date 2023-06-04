
// constants used for calculations
const float EQUIRECT_PI = 3.14159265;
const float EQUIRECT_INV_TWO_PI = 1.0 / ( EQUIRECT_PI * 2.0 );
const float EQUIRECT_INV_HALF_PI = 1.0 / ( EQUIRECT_PI * 0.5 );
const float EQUIRECT_INV_PI = 1.0 / EQUIRECT_PI;

// calculate equirectangular coordinates from normal. normal has to be normalized
vec2 equirectFromNormal( in vec3 normal ){
	// NOTE atan(x,y) is undefined for x=0. so far I did not see any problems though
	return vec2(
		atan( normal.x, normal.z ) * EQUIRECT_INV_TWO_PI + 0.5,
		0.5 - asin( normal.y ) * EQUIRECT_INV_PI );
}
