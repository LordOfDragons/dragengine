
// constants
//////////////

const int PcfMode1Tap = 0;
const int PcfMode4Tap = 1;
const int PcfMode9Tap = 2;
const int PcfModeVarTap = 3;

const vec4 pcf4TapTCScale = vec4(0.5, 0.5, -0.5, -0.5);
const float pcf4TapWeight = 0.25;

const vec4 pcf9TapTCScale = vec4(1.0, 1.0, -1.0, -1.0);
const vec4 pcf9TapTCScale2 = vec4(1.0, 1.0, 0.0, 0.0);
const float pcf9TapWeight = 1.0 / 9.0;

const int ShadowModeMap = 0;
const int ShadowModeCube = 1;
const int ShadowModeDualParaboloid = 2;
const int ShadowModePyramid = 2;


// noise
//////////

#ifndef SHADOW_NOISE_TEXTURE
#define SHADOW_NOISE_TEXTURE texShadowNoise
#endif

const vec2 shadowNoiseTapScale = vec2(1.0);
const vec2 shadowNoiseTapOffset = vec2(-0.5);

vec2 sampleShadowNoise(in ARG_CONST vec2 tc){
	return vec2(texture(SHADOW_NOISE_TEXTURE, tc))
		* shadowNoiseTapScale + shadowNoiseTapOffset;
}

vec2 sampleShadowNoise(in ARG_CONST vec2 tc, in ARG_CONST vec2 tcscale){
	return sampleShadowNoise(tc) * tcscale;
}

/*
vec2 sampleShadowNoise(in ARG_CONST vec2 tc, in ARG_CONST ivec2 tcoffset){
	return vec2(textureOffset(SHADOW_NOISE_TEXTURE, tc, tcoffset))
		* shadowNoiseTapScale + shadowNoiseTapOffset;
}
*/
#define inl_sampleShadowNoise_offset(a,b) (vec2(textureOffset(SHADOW_NOISE_TEXTURE,a,b)) * shadowNoiseTapScale + shadowNoiseTapOffset)

/*
vec2 sampleShadowNoise(in ARG_CONST vec2 tc, in ARG_CONST ivec2 tcoffset, in ARG_CONST vec2 tcscale){
	return sampleShadowNoise(tc, tcoffset) * tcscale;
}
*/
#define inl_sampleShadowNoise_offset2(a,b,c) (inl_sampleShadowNoise_offset(a,b) * c)


// 2d shadow map
//////////////////

float sampleShadowMap(in ARG_SAMP_HIGHP sampler2DShadow texsm,
in vec3 tc, in ARG_CONST vec2 tcnoise, in ARG_CONST vec2 tcnoiseScale){
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseScale);
	}
	return texture(texsm, tc);
}

/*
float sampleShadowMap(in ARG_SAMP_HIGHP sampler2DShadow texsm, in vec3 tc, in ARG_CONST vec2 tcoffset,
in ARG_CONST vec2 tcnoise, in ARG_CONST ivec2 tcnoiseOffset, in ARG_CONST vec2 tcnoiseScale){
	tc.st += tcoffset;
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseOffset, tcnoiseScale);
	}
	return texture(texsm, tc);
}
*/
#define inl_sampleShadowMap(a,b,c,d,e,f) texture(a, b + vec3(c,0) + (NoiseTap ? vec3(inl_sampleShadowNoise_offset2(d,e,f),0) : vec3(0)))

float evalShadowMap(in ARG_SAMP_HIGHP sampler2DShadow texsm, in ARG_CONST vec3 params, in ARG_CONST vec3 position){
	#define ES2DTC vec3
	#define ES2D(tc) vec3(tc)
	#define SSM sampleShadowMap
	#define SSM2(a,b,c,d,e,f) inl_sampleShadowMap(a,b,c,d,e,f)
	#include "shared/defren/light/shadow/shadow_impl_map.glsl"
	return shadow;
}


// 2d shadow map array
////////////////////////

float sampleShadowMapArray(in ARG_SAMP_HIGHP sampler2DArrayShadow texsm,
in vec4 tc, in ARG_CONST vec2 tcnoise, in ARG_CONST vec2 tcnoiseScale){
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseScale);
	}
	return texture(texsm, tc);
}

/*
float sampleShadowMapArray(in ARG_SAMP_HIGHP sampler2DArrayShadow texsm,
in vec4 tc, in ARG_CONST vec2 tcoffset, in ARG_CONST vec2 tcnoise,
in ARG_CONST ivec2 tcnoiseOffset, in ARG_CONST vec2 tcnoiseScale){
	tc.st += tcoffset;
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseOffset, tcnoiseScale);
	}
	return texture(texsm, tc);
}
*/
#define inl_sampleShadowMapArray(a,b,c,d,e,f) texture(a, b + vec4(c,0,0) + (NoiseTap ? vec4(inl_sampleShadowNoise_offset2(d,e,f),0,0) : vec4(0)))

float evalShadowMapArray(in ARG_SAMP_HIGHP sampler2DArrayShadow texsm,
in ARG_CONST vec3 params, in ARG_CONST vec4 position){
	#define ES2DTC vec4
	#define ES2D(tc) tc
	#define SSM sampleShadowMapArray
	#define SSM2(a,b,c,d,e,f) inl_sampleShadowMapArray(a,b,c,d,e,f)
	#include "shared/defren/light/shadow/shadow_impl_map.glsl"
	return shadow;
}


// shadow cube
////////////////

float sampleShadowCube(in ARG_SAMP_HIGHP samplerCubeShadow texsm,
in vec4 tc, in ARG_CONST vec2 tcnoise, in ARG_CONST mat2x3 tcnoiseMatrix){
	if(NoiseTap){
		tc.stp += tcnoiseMatrix * sampleShadowNoise(tcnoise);
	}
	return texture(texsm, tc);
}

float sampleShadowCube(in ARG_SAMP_HIGHP samplerCubeShadow texsm, in vec4 tc,
in ARG_CONST vec3 tcoffset, in ARG_CONST vec2 tcnoise, in ARG_CONST ivec2 tcnoiseOffset,
in ARG_CONST mat2x3 tcnoiseMatrix){
	tc.stp += tcoffset;
	if(NoiseTap){
		tc.stp += tcnoiseMatrix * sampleShadowNoise(tcnoise, tcnoiseOffset);
	}
	return texture(texsm, tc);
}

float evalShadowCube(in ARG_SAMP_HIGHP samplerCubeShadow texsm,
in ARG_CONST vec3 params, in ARG_CONST vec4 position){
	#define SSM sampleShadowCube
	#include "shared/defren/light/shadow/shadow_impl_cube.glsl"
	return shadow;
}



// shadow thickness
/////////////////////

// distanceBlocker2Fragment = distanceFragmentToLight * (1 - depthBlocker / depthFragment)
// this works only with linear shadow maps. with projective shadow maps the depths have to
// be first back-transformed into distances where a simple subtraction can then be done

float shadowMapThickness(in ARG_SAMP_HIGHP sampler2D texsm,
in ARG_CONST vec3 position, in ARG_CONST vec2 depthTransform){
	float thickness = texture(texsm, vec2(position)).r;
	thickness = depthTransform.x / (depthTransform.y - thickness);
	thickness = (depthTransform.x / (depthTransform.y - position.p)) - thickness;
	return max(thickness, 0.0);
}

float shadowMapArrayThickness(in ARG_SAMP_HIGHP sampler2DArray texsm,
in ARG_CONST float scale, in ARG_CONST vec4 position){
	float thickness = texture(texsm, vec3(position)).r * scale;
	thickness = position.q * scale - thickness;
	return max(thickness, 0.0);
}

float shadowCubeThickness(in ARG_SAMP_HIGHP samplerCube texsm,
in ARG_CONST vec4 position, in ARG_CONST vec2 depthTransform){
	float thickness = texture(texsm, vec3(position)).r;
	thickness = thickness * depthTransform.x + depthTransform.y;
	thickness = position.q - thickness;
	return max(thickness, 0.0);
}
