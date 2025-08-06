// normal encoding and decoding

/*
	<!-- shared/normal_texture.glsl -->
	<define>MATERIAL_NORMAL_DEC</define>
	<define>MATERIAL_NORMAL_ENC</define>
*/

#include "shared/normal/constants.glsl"


// load normal from sampler using direct method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
vec3 normalLoadDirect( in ARG_SAMP_MEDP sampler2D sampler, in ivec2 texCoord ){
	return vec3( texelFetch( sampler, texCoord, 0 ) );
}

vec3 normalLoadDirect( in ARG_SAMP_MEDP sampler2DArray sampler, in ivec3 texCoord ){
	return vec3( texelFetch( sampler, texCoord, 0 ) );
}

vec3 normalLoadDirect( in ARG_SAMP_MEDP sampler2D sampler, in vec2 texCoord ){
	return vec3( textureLod( sampler, texCoord, 0.0 ) );
}

vec3 normalLoadDirect( in ARG_SAMP_MEDP sampler2DArray sampler, in vec3 texCoord ){
	return vec3( textureLod( sampler, texCoord, 0.0 ) );
}


// decode normal using shifted floating point method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
vec3 normalDecodeShiftedFloat( in vec3 encoded ){
	return encoded * vec3( 2 ) + vec3( -1 );
}

// load normal from sampler using shifted floating point method.
// use this method for RGB16F, RGB32F, RGB8_SNORM or RGB16_SNORM textures.
// normal is potentially not normalized.
vec3 normalLoadShiftedFloat( in ARG_SAMP_MEDP sampler2D sampler, in ivec2 texCoord ){
	return normalDecodeShiftedFloat( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedFloat( in ARG_SAMP_MEDP sampler2DArray sampler, in ivec3 texCoord ){
	return normalDecodeShiftedFloat( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedFloat( in ARG_SAMP_MEDP sampler2D sampler, in vec2 texCoord ){
	return normalDecodeShiftedFloat( vec3( textureLod( sampler, texCoord, 0.0 ) ) );
}

vec3 normalLoadShiftedFloat( in ARG_SAMP_MEDP sampler2DArray sampler, in vec3 texCoord ){
	return normalDecodeShiftedFloat( vec3( textureLod( sampler, texCoord, 0.0 ) ) );
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
vec3 normalLoadShiftedInt( in ARG_SAMP_MEDP sampler2D sampler, in ivec2 texCoord ){
	return normalDecodeShiftedInt( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedInt( in ARG_SAMP_MEDP sampler2DArray sampler, in ivec3 texCoord ){
	return normalDecodeShiftedInt( vec3( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadShiftedInt( in ARG_SAMP_MEDP sampler2D sampler, in vec2 texCoord ){
	return normalDecodeShiftedInt( vec3( textureLod( sampler, texCoord, 0.0 ) ) );
}

vec3 normalLoadShiftedInt( in ARG_SAMP_MEDP sampler2DArray sampler, in vec3 texCoord ){
	return normalDecodeShiftedInt( vec3( textureLod( sampler, texCoord, 0.0 ) ) );
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

// load normal from sampler using sphere map method.
// use this method for RG8 or RG16 textures.
// normal is potentially not normalized.
vec3 normalLoadSphereMap( in ARG_SAMP_MEDP sampler2D sampler, in ivec2 texCoord ){
	return normalDecodeSphereMap( vec2( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadSphereMap( in ARG_SAMP_MEDP sampler2DArray sampler, in ivec3 texCoord ){
	return normalDecodeSphereMap( vec2( texelFetch( sampler, texCoord, 0 ) ) );
}

vec3 normalLoadSphereMap( in ARG_SAMP_MEDP sampler2D sampler, in vec2 texCoord ){
	return normalDecodeSphereMap( vec2( textureLod( sampler, texCoord, 0.0 ) ) );
}

vec3 normalLoadSphereMap( in ARG_SAMP_MEDP sampler2DArray sampler, in vec3 texCoord ){
	return normalDecodeSphereMap( vec2( textureLod( sampler, texCoord, 0.0 ) ) );
}

// encode normal using sphere map method.
// use this method for RG8 or RG16 textures. normal has to be normalized.
vec2 normalEncodeSphereMap( in vec3 normal ){
	return normal.xy / vec2( sqrt( 8.001 - 7.999 * normal.z ) ) + vec2( 0.5 );
}


// sanitize normal. if normal is near 0 returns (0,0,1). otherwise normalizes the normal
vec3 sanitizeNormal( in vec3 normal ){
	return length( normal ) > 0.001 ? normalize( normal ) : vec3( 0, 0, 1 );
}


// load normal using method decided by using one of these preprocessor definitions
// - MaterialNormalDec = MaterialNormalModeFloatBasic: normalLoadShiftedFloat
// - MaterialNormalDec = MaterialNormalModeIntBasic: normalLoadShiftedInt
// - MaterialNormalDec = MaterialNormalModeSpheremap: normalLoadSphere
// - MaterialNormalDec = MaterialNormalModeFloat: normalLoadDirect
// normal is potentially not normalized.
vec3 normalLoadMaterial( in ARG_SAMP_MEDP sampler2D sampler, in ivec2 texCoord ){
	if(MaterialNormalDec == MaterialNormalModeFloatBasic){
		return normalLoadShiftedFloat( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeIntBasic){
		return normalLoadShiftedInt( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeSpheremap){
		return normalLoadSphereMap( sampler, texCoord );
		
	}else{
		return normalLoadDirect( sampler, texCoord );
	}
}

vec3 normalLoadMaterial( in ARG_SAMP_MEDP sampler2DArray sampler, in ivec3 texCoord ){
	if(MaterialNormalDec == MaterialNormalModeFloatBasic){
		return normalLoadShiftedFloat( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeIntBasic){
		return normalLoadShiftedInt( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeSpheremap){
		return normalLoadSphereMap( sampler, texCoord );
		
	}else{
		return normalLoadDirect( sampler, texCoord );
	}
}

vec3 normalLoadMaterial( in ARG_SAMP_MEDP sampler2D sampler, in vec2 texCoord ){
	if(MaterialNormalDec == MaterialNormalModeFloatBasic){
		return normalLoadShiftedFloat( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeIntBasic){
		return normalLoadShiftedInt( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeSpheremap){
		return normalLoadSphereMap( sampler, texCoord );
		
	}else{
		return normalLoadDirect( sampler, texCoord );
	}
}

vec3 normalLoadMaterial( in ARG_SAMP_MEDP sampler2DArray sampler, in vec3 texCoord ){
	if(MaterialNormalDec == MaterialNormalModeFloatBasic){
		return normalLoadShiftedFloat( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeIntBasic){
		return normalLoadShiftedInt( sampler, texCoord );
		
	}else if(MaterialNormalDec == MaterialNormalModeSpheremap){
		return normalLoadSphereMap( sampler, texCoord );
		
	}else{
		return normalLoadDirect( sampler, texCoord );
	}
}

// encode normal using method decided by using one of these preprocessor definitions
// - MaterialNormalEnc = MaterialNormalModeFloatBasic: normalEncodeShiftedFloat
// - MaterialNormalEnc = MaterialNormalModeIntBasic: normalEncodeShiftedInt
// - MaterialNormalEnc = MaterialNormalModeSpheremap: normalEncodeSphereMap
// - MaterialNormalDec = MaterialNormalModeFloat: normal unmodified
// normal has to be normalized. encoded value is padded to vec3 with 0
vec3 normalEncodeMaterial( in vec3 normal ){
	if(MaterialNormalEnc == MaterialNormalModeFloatBasic){
		return normalEncodeShiftedFloat( normal );
		
	}else if(MaterialNormalEnc == MaterialNormalModeIntBasic){
		return normalEncodeShiftedInt( normal );
		
	}else if(MaterialNormalEnc == MaterialNormalModeSpheremap){
		return vec3( normalEncodeSphereMap( normal ), 0.0 );
		
	}else{
		return normal;
	}
}
