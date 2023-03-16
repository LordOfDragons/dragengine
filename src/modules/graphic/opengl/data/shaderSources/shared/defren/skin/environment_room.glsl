
// calculate environment room coordinates.
// 
// vReflectDir
//   calculated by vertex shader. this is the view vector for the current fragment
// 
// normal
//   the surface normal as calculated by fragment shader. this can differ from vNormal
//   calculated by the vertex shader if normal mapping is used which changes the normal
// 
// vTangent
//   calculated by vertex shader. this is the tangent vector of the surface.
// 
// vBitangent
//   calculated by vertex shader. this is the bi-tangent vector of the surface.
// 
// tc
//   texture coordinates calculated by fragment shader. this can differ from if relief
//   mapping is used.
// 
// pEnvRoomSize
//   size of the environment room. X = width / height. Y = -( width / depth )
// 
// pEnvRoomOffset
//   offset of environment room cube map origin relative to room center
// 
// The texture coordinate defines where the view vector enters the room while the vReflectDir
// defines the view vector direction. Using these two values the hit point with the box behind
// the surface can be calculated. The final room direction is then the difference compared to
// the room center
vec3 calcEnvRoomDir( in vec2 tc, in vec3 normal ){
	// transform view vector into vector relative to room. the resulting room vector is
	// pointing from the hit point on the surface into the room behind the surface
	vec3 roomDir = vec3(
		dot( vTangent, vReflectDir ),
		dot( vBitangent, vReflectDir ),
		dot( normal, vReflectDir ) );
	
	// pEnvRoomSize is chosen to normalize the vector relative to the room width. this
	// results in a vector relative to the unit cube. converting to a unit cube now than
	// later makes it easier to do the calculation
	roomDir.yz *= pEnvRoomSize;
	
	// we need now to calculate the hit point of the view vector with the unit cube. for this
	// we need first the position on the unit cube the view vector enters the cube. we only
	// use the fractional part of the texture coordinate to allow multiple copies of rooms
	// to be rendered if the texture coordinate exceeds the 0..1 range. the (0.5, 0.5)
	// texture coordinate is the center of the unit cube representing the surface
	vec3 enterPos = vec3( fract( tc ) * vec2( 2.0 ) - vec2( 1.0 ), -1.0 );
	
	// offset the enter position to match the true cube map origin. this offset is required
	// if the cube map origin is not located exactly at the center of the room. this is
	// typically the case for in-engine rendered cube maps used as environment room cube maps.
	// placing the cube map origin near the entry side of the room reduces perspective
	// problems. by offseting the enter position we calculate the room direction vector
	// against this original position instead of the room center.
	enterPos += pEnvRoomOffset;
	
	// the following calculations are an optimized version of ray-plane intersection tests.
	// 5 planes are tested since the one lining up with the surface can never be hit. the
	// hit with the smallest lambda is chosen.
	// 
	// the first block does ray-plane intersection tests with the positive X, Y and Z axis.
	// the second block does ray-plane intersection tests with the negative X and Y axis.
	vec3 divider = mix( vec3( 1.0 ) / roomDir, vec3( 1000.0 ),
		lessThan( abs( roomDir ), vec3( 0.0001 ) ) );
	
	vec4 lambda;
	
	lambda.xyz = ( vec3( 1.0 ) - enterPos ) * divider;
	lambda.xyz = mix( lambda.xyz, vec3( 1000.0 ), lessThan( lambda.xyz, vec3( 0.0 ) ) );
	lambda.y = min( lambda.y, lambda.z );
	
	lambda.zw = ( vec2( -1.0 ) - enterPos.xy ) * divider.xy;
	lambda.zw = mix( lambda.zw, vec2( 1000.0 ), lessThan( lambda.zw, vec2( 0.0 ) ) );
	lambda.xy = min( lambda.xy, lambda.zw );
	
	lambda.x = min( lambda.x, lambda.y );
	
	// lambda.x is the smallest lambda. with this the final vector can be calculated
	roomDir = roomDir * vec3( lambda.x ) + enterPos;
	roomDir.y = -roomDir.y;
	
	return roomDir;
}
