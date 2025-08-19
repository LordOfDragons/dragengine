// normal encoding and decoding

/*
	<!-- shared/normal_image.glsl -->
	<define>MATERIAL_NORMAL_DEC</define>
	<define>MATERIAL_NORMAL_ENC</define>
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
// - MaterialNormalDec = MaterialNormalModeFloatBasic: normalLoadShiftedFloat
// - MaterialNormalDec = MaterialNormalModeIntBasic: normalLoadShiftedInt
// - MaterialNormalDec = MaterialNormalModeSpheremap: normalLoadSphere
// - MaterialNormalDec = MaterialNormalModeFloat: normalLoadDirect
// normal is potentially not normalized.
vec3 normalLoadMaterial( readonly image2D image, in ivec2 texCoord ){
	if(MaterialNormalDec == MaterialNormalModeFloatBasic){
		return normalLoadShiftedFloat( image, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeIntBasic){
		return normalLoadShiftedInt( image, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeSpheremap){
		return normalLoadSphereMap( image, texCoord );
		
	}else{ // MaterialNormalDec == MaterialNormalModeFloat
		return normalLoadDirect( image, texCoord );
	}
}

vec3 normalLoadMaterial( readonly image2DArray image, in ivec3 texCoord ){
	if(MaterialNormalDec == MaterialNormalModeFloatBasic){
		return normalLoadShiftedFloat( image, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeIntBasic){
		return normalLoadShiftedInt( image, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeSpheremap){
		return normalLoadSphereMap( image, texCoord );
		
	}else{ // MaterialNormalDec == MaterialNormalModeFloat
		return normalLoadDirect( image, texCoord );
	}
}

// encode normal using method decided by using one of these preprocessor definitions
// - MaterialNormalDec = MaterialNormalModeFloatBasic: normalEncodeShiftedFloat
// - MaterialNormalDec = MaterialNormalModeIntBasic: normalEncodeShiftedInt
// - MaterialNormalDec = MaterialNormalModeSpheremap: normalEncodeSphereMap
// - MaterialNormalDec = MaterialNormalModeFloat: normal unmodified
// normal has to be normalized. encoded value is padded to vec3 with 0
vec3 normalEncodeMaterial( in vec3 normal ){
	if(MaterialNormalDec == MaterialNormalModeFloatBasic){
		return normalEncodeShiftedFloat( normal );
		
	}else if(MaterialNormalDec == MaterialNormalModeIntBasic){
		return normalEncodeShiftedInt( normal );
		
	}else if(MaterialNormalDec == MaterialNormalModeSpheremap){
		return vec3( normalEncodeSphereMap( normal ), 0 );
		
	}else{ // MaterialNormalDec == MaterialNormalModeFloat
		return normal;
	}
}
