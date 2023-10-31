
/**
 * Prevent light from exceeding safe values.
 */
vec3 sanitizeLight( in vec3 light ){
	return clamp( light, vec3( 0 ), vec3( 1000 ) );
}


/**
 * Prevent light from exceeding safe values and protects against NaN values.
 */
vec3 sanitizeLightNaN( in vec3 light ){
	if( isnan( light.r ) ){
		light.r = 0;
	}
	if( isnan( light.g ) ){
		light.g = 0;
	}
	if( isnan( light.b ) ){
		light.b = 0;
	}
	
	return sanitizeLight( light );
}
