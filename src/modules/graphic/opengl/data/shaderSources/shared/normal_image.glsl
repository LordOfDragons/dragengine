// normal encoding and decoding

/*
	<!-- shared/normal_image.glsl -->
	<define>MATERIAL_NORMAL_DEC_FLOATBASIC</define>
	<define>MATERIAL_NORMAL_DEC_INTBASIC</define>
	<define>MATERIAL_NORMAL_DEC_SPHEREMAP</define>
	<define>MATERIAL_NORMAL_ENC_FLOATBASIC</define>
	<define>MATERIAL_NORMAL_ENC_INTBASIC</define>
	<define>MATERIAL_NORMAL_ENC_SPHEREMAP</define>
*/

// load normal from image using direct method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
vec3 normalLoadDirect( readonly image2D image, in ivec2 texCoord ){
	return vec3( imageLoad( image, texCoord ) );
}

vec3 normalLoadDirect( readonly image2DArray image, in ivec3 texCoord ){
	return vec3( imageLoad( image, texCoord ) );
}


// decode normal using shifted floating point method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
// normal is potentially not normalized.
vec3 normalDecodeShiftedFloat( in vec3 encoded ){
	return encoded * vec3( 2 ) + vec3( -1 );
}

// load normal from image using shifted floating point method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
vec3 normalLoadShiftedFloat( readonly image2D image, in ivec2 texCoord ){
	return normalDecodeShiftedFloat( vec3( imageLoad( image, texCoord ) ) );
}

vec3 normalLoadShiftedFloat( readonly image2DArray image, in ivec3 texCoord ){
	return normalDecodeShiftedFloat( vec3( imageLoad( image, texCoord ) ) );
}

// encode normal using shifted floating point method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal has to be normalized.
vec3 normalEncodeShiftedFloat( in vec3 normal ){
	return normal * vec3( 0.5 ) + vec3( 0.5 );
}


// decode normal using shifted integer method.
// use this method for RGB8 or RGB16 textures.
// normal is potentially not normalized.
vec3 normalDecodeShiftedInt( in vec3 encoded ){
	return encoded * vec3( 1.9921569 ) + vec3( -0.9921722 );
}

// load normal from image using shifted integer method.
// use this method for RGB8 or RGB16 textures.
vec3 normalLoadShiftedInt( readonly image2D image, in ivec2 texCoord ){
	return normalDecodeShiftedInt( vec3( imageLoad( image, texCoord ) ) );
}

vec3 normalLoadShiftedInt( readonly image2DArray image, in ivec3 texCoord ){
	return normalDecodeShiftedInt( vec3( imageLoad( image, texCoord ) ) );
}

// encode normal using shifted integer method.
// use this method for RGB8 or RGB16 textures. normal has to be normalized.
vec3 normalEncodeShiftedInt( in vec3 normal ){
	return normal * vec3( 0.5 ) + vec3( 0.5 );
}


// decode normal using sphere map method.
// use this method for RG8 or RG16 textures.
// normal is potentially not normalized.
vec3 normalDecodeSphereMap( in vec2 encoded ){
	vec2 fenc = encoded * vec2( 4 ) - vec2( 2 );
	float f = dot( fenc, fenc );
	float g = sqrt( 1.0 - f * 0.25 );
	return vec3( fenc.xy * vec2( g ), f * 0.5 - 1.0 );
}

// load normal from image using sphere map method.
// use this method for RG8 or RG16 textures.
// normal is potentially not normalized.
vec3 normalLoadSphereMap( readonly image2D image, in ivec2 texCoord ){
	return normalDecodeSphereMap( vec2( imageLoad( image, texCoord ) ) );
}

vec3 normalLoadSphereMap( readonly image2DArray image, in ivec3 texCoord ){
	return normalDecodeSphereMap( vec2( imageLoad( image, texCoord ) ) );
}

// encode normal using sphere map method.
// use this method for RG8 or RG16 textures. normal has to be normalized.
vec2 normalEncodeSphereMap( in vec3 normal ){
	return normal.xy / vec2( sqrt( 8.001 - 7.999 * normal.z ) ) + vec2( 0.5 );
}


// load normal using method decided by using one of these preprocessor definitions
// - MATERIAL_NORMAL_DEC_FLOATBASIC: normalLoadShiftedFloat
// - MATERIAL_NORMAL_DEC_INTBASIC: normalLoadShiftedInt
// - MATERIAL_NORMAL_DEC_SPHEREMAP: normalLoadSphere
// - none of the above: normalLoadDirect
// normal is potentially not normalized.
vec3 normalLoadMaterial( readonly image2D image, in ivec2 texCoord ){
	#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
	return normalLoadShiftedFloat( image, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_INTBASIC
	return normalLoadShiftedInt( image, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
	return normalLoadSphereMap( image, texCoord );
	
	#else
	return normalLoadDirect( image, texCoord );
	#endif
}

vec3 normalLoadMaterial( readonly image2DArray image, in ivec3 texCoord ){
	#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
	return normalLoadShiftedFloat( image, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_INTBASIC
	return normalLoadShiftedInt( image, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
	return normalLoadSphereMap( image, texCoord );
	
	#else
	return normalLoadDirect( image, texCoord );
	#endif
}

// encode normal using method decided by using one of these preprocessor definitions
// - MATERIAL_NORMAL_ENC_FLOATBASIC: normalEncodeShiftedFloat
// - MATERIAL_NORMAL_ENC_INTBASIC: normalEncodeShiftedInt
// - MATERIAL_NORMAL_ENC_SPHEREMAP: normalEncodeSphereMap
// - none of the above: normal unmodified
// normal has to be normalized. encoded value is padded to vec3 with 0
vec3 normalEncodeMaterial( in vec3 normal ){
	#ifdef MATERIAL_NORMAL_ENC_FLOATBASIC
	return normalEncodeShiftedFloat( normal );
	
	#elif defined MATERIAL_NORMAL_ENC_INTBASIC
	return normalEncodeShiftedInt( normal );
	
	#elif defined MATERIAL_NORMAL_ENC_SPHEREMAP
	return vec3( normalEncodeSphereMap( normal ), 0 );
	
	#else
	return normal;
	#endif
}
