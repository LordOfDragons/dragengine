// octahedral mapping used for DDGI type rendering

vec2 octahderalSignNotZero( in vec2 v ){
	return vec2( v.x >= 0.0 ? 1.0 : -1.0, v.y >= 0.0 ? 1.0 : -1.0 );
}

// transform unit vector to octahedral vector in the range [-1 .. 1]
vec2 octahderalEncode( in vec3 v ){
	vec3 vabs = abs( v );
	vec2 result = v.xy / ( vabs.x + vabs.y + vabs.z );
	return v.z < 0.0 ? ( 1.0 - abs( result.yx ) ) * octahderalSignNotZero( result.xy ) : result;
}

// transform octahedral vector in the range [-1 .. 1] to unit vector
vec3 octahedralDecode( in vec2 v ){
	vec2 vabs = abs( v.xy );
	vec3 result = vec3( v.x, v.y, 1.0 - vabs.x - vabs.y );
	if( result.z < 0.0 ){
		result.xy = ( 1.0 - abs( result.yx ) ) * octahderalSignNotZero( result.xy );
	}
	return normalize( result );
}
