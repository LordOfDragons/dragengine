// normal encoding and decoding


// load normal from sampler using direct method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
vec3 normalLoadDirect( in sampler2D sampler, in ivec2 texCoord ){
	return vec3( texelFetch( sampler, texCoord, 0 ) );
}

vec3 normalLoadDirect( in sampler2DArray sampler, in ivec3 texCoord ){
	return vec3( texelFetch( sampler, texCoord, 0 ) );
}

vec3 normalLoadDirect( in sampler2D sampler, in vec2 texCoord ){
	return vec3( texture( sampler, texCoord, 0 ) );
}


// decode normal using shifted floating point method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
// normal is potentially not normalized.
vec3 normalDecodeShiftedFloat( in vec3 encoded ){
	return encoded * vec3( 2.0 ) + vec3( -1.0 );
}

// load normal from sampler using shifted floating point method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
vec3 normalLoadShiftedFloat( in sampler2D sampler, in ivec2 texCoord ){
	return normalDecodeShiftedFloat( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedFloat( in sampler2DArray sampler, in ivec3 texCoord ){
	return normalDecodeShiftedFloat( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedFloat( in sampler2D sampler, in vec2 texCoord ){
	return normalDecodeShiftedFloat( vec3( texture( sampler, texCoord, 0 ) ) );
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

// load normal from sampler using shifted integer method.
// use this method for RGB8 or RGB16 textures.
vec3 normalLoadShiftedInt( in sampler2D sampler, in ivec2 texCoord ){
	return normalDecodeShiftedInt( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedInt( in sampler2DArray sampler, in ivec3 texCoord ){
	return normalDecodeShiftedInt( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedInt( in sampler2D sampler, in vec2 texCoord ){
	return normalDecodeShiftedInt( vec3( texture( sampler, texCoord, 0 ) ) );
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
	vec2 fenc = encoded * vec2( 4.0 ) - vec2( 2.0 );
	float f = dot( fenc, fenc );
	float g = sqrt( 1.0 - f * 0.25 );
	return vec3( fenc.xy * vec2( g ), f * 0.5 - 1.0 );
}

// load normal from sampler using sphere map method.
// use this method for RG8 or RG16 textures.
// normal is potentially not normalized.
vec3 normalLoadSphereMap( in sampler2D sampler, in ivec2 texCoord ){
	return normalDecodeSphereMap( vec2( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadSphereMap( in sampler2DArray sampler, in ivec3 texCoord ){
	return normalDecodeSphereMap( vec2( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadSphereMap( in sampler2D sampler, in vec2 texCoord ){
	return normalDecodeSphereMap( vec2( texture( sampler, texCoord, 0 ) ) );
}

// encode normal using sphere map method.
// use this method for RG8 or RG16 textures. normal has to be normalized.
vec2 normalEncodeSphereMap( in vec3 normal ){
	return normal.xy / vec2( sqrt( 8.0001 - 7.9999 * normal.z ) ) + vec2( 0.5 );
}


// load normal using method decided by using one of these preprocessor definitions
// - MATERIAL_NORMAL_DEC_FLOATBASIC: normalLoadShiftedFloat
// - MATERIAL_NORMAL_DEC_INTBASIC: normalLoadShiftedInt
// - MATERIAL_NORMAL_DEC_SPHEREMAP: normalLoadSphere
// - none of the above: normalLoadDirect
// normal is potentially not normalized.
vec3 normalLoadMaterial( in sampler2D sampler, in ivec2 texCoord ){
	#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
	return normalLoadShiftedFloat( sampler, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_INTBASIC
	return normalLoadShiftedInt( sampler, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
	return normalLoadSphereMap( sampler, texCoord );
	
	#else
	return normalLoadDirect( sampler, texCoord );
	#endif
}

vec3 normalLoadMaterial( in sampler2DArray sampler, in ivec3 texCoord ){
	#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
	return normalLoadShiftedFloat( sampler, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_INTBASIC
	return normalLoadShiftedInt( sampler, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
	return normalLoadSphereMap( sampler, texCoord );
	
	#else
	return normalLoadDirect( sampler, texCoord );
	#endif
}

vec3 normalLoadMaterial( in sampler2D sampler, in vec2 texCoord ){
	#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
	return normalLoadShiftedFloat( sampler, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_INTBASIC
	return normalLoadShiftedInt( sampler, texCoord );
	
	#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
	return normalLoadSphereMap( sampler, texCoord );
	
	#else
	return normalLoadDirect( sampler, texCoord );
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
	return vec3( normalEncodeSphereMap( normal ), 0.0 );
	
	#else
	return normal;
	#endif
}


// zero-normal encoded using respective method. some methods can not represent
// a true zero normal. in this case (0,0,-1) is used
const vec3 normalZeroShiftedFloat = vec3( 0.5 );
const vec3 normalZeroShiftedInt = vec3( 127.0 / 255.0 );
const vec2 normalZeroSphereMap = vec2( 0.5 );
const vec3 normalZeroDirect = vec3( 0.0 );

// zero-normal encoded using respective method. some methods can not represent a true
// zero normal. in this case (0,0,-1) is used. constant is padded to vec3 with 0
#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
const vec3 normalZeroMaterialDec = normalZeroShiftedFloat;

#elif defined MATERIAL_NORMAL_DEC_INTBASIC
const vec3 normalZeroMaterialDec = normalZeroShiftedInt;

#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
const vec3 normalZeroMaterialDec = vec3( normalZeroSphereMap, 0.0 );

#else
const vec3 normalZeroMaterialDec = normalZeroDirect;
#endif

#ifdef MATERIAL_NORMAL_DEC_FLOATBASIC
const vec3 normalZeroMaterialEnc = normalZeroShiftedFloat;

#elif defined MATERIAL_NORMAL_DEC_INTBASIC
const vec3 normalZeroMaterialEnc = normalZeroShiftedInt;

#elif defined MATERIAL_NORMAL_DEC_SPHEREMAP
const vec3 normalZeroMaterialEnc = vec3( normalZeroSphereMap, 0.0 );

#else
const vec3 normalZeroMaterialEnc = normalZeroDirect;
#endif
