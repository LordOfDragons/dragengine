
// constants
//////////////

const vec4 pcf4TapTCScale = vec4(0.5, 0.5, -0.5, -0.5);
const float pcf4TapWeight = 0.25;

const vec4 pcf9TapTCScale = vec4(1.0, 1.0, -1.0, -1.0);
const vec4 pcf9TapTCScale2 = vec4(1.0, 1.0, 0.0, 0.0);
const float pcf9TapWeight = 1.0 / 9.0;


// noise
//////////

#ifndef SHADOW_NOISE_TEXTURE
#define SHADOW_NOISE_TEXTURE texShadowNoise
#endif

const vec2 shadowNoiseTapScale = vec2(1.0);
const vec2 shadowNoiseTapOffset = vec2(-0.5);

vec2 sampleShadowNoise(const in vec2 tc){
	return vec2(texture(SHADOW_NOISE_TEXTURE, tc))
		* shadowNoiseTapScale + shadowNoiseTapOffset;
}

vec2 sampleShadowNoise(const in vec2 tc, const in vec2 tcscale){
	return sampleShadowNoise(tc) * tcscale;
}

/*
vec2 sampleShadowNoise(const in vec2 tc, const in ivec2 tcoffset){
	return vec2(textureOffset(SHADOW_NOISE_TEXTURE, tc, tcoffset))
		* shadowNoiseTapScale + shadowNoiseTapOffset;
}
*/
#define inl_sampleShadowNoise_offset(a,b) (vec2(textureOffset(SHADOW_NOISE_TEXTURE,a,b)) * shadowNoiseTapScale + shadowNoiseTapOffset)

/*
vec2 sampleShadowNoise(const in vec2 tc, const in ivec2 tcoffset, const in vec2 tcscale){
	return sampleShadowNoise(tc, tcoffset) * tcscale;
}
*/
#define inl_sampleShadowNoise_offset2(a,b,c) (inl_sampleShadowNoise_offset(a,b) * c)


// 2d shadow map
//////////////////

float sampleShadowMap(in ARG_SAMP_HIGHP sampler2DShadow texsm,
in vec3 tc, const in vec2 tcnoise, const in vec2 tcnoiseScale){
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseScale);
	}
	return texture(texsm, tc);
}

/*
float sampleShadowMap(in ARG_SAMP_HIGHP sampler2DShadow texsm, in vec3 tc, const in vec2 tcoffset,
const in vec2 tcnoise, const in ivec2 tcnoiseOffset, const in vec2 tcnoiseScale){
	tc.st += tcoffset;
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseOffset, tcnoiseScale);
	}
	return texture(texsm, tc);
}
*/
#define inl_sampleShadowMap(a,b,c,d,e,f) texture(a, b + vec3(c,0.0) + (NoiseTap ? vec3(inl_sampleShadowNoise_offset2(d,e,f),0.0) : vec3(0.0)))

float evalShadowMap(in ARG_SAMP_HIGHP sampler2DShadow texsm, const in vec3 params, const in vec3 position){
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
in vec4 tc, const in vec2 tcnoise, const in vec2 tcnoiseScale){
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseScale);
	}
	return texture(texsm, tc);
}

/*
float sampleShadowMapArray(in ARG_SAMP_HIGHP sampler2DArrayShadow texsm,
in vec4 tc, const in vec2 tcoffset, const in vec2 tcnoise,
const in ivec2 tcnoiseOffset, const in vec2 tcnoiseScale){
	tc.st += tcoffset;
	if(NoiseTap){
		tc.st += sampleShadowNoise(tcnoise, tcnoiseOffset, tcnoiseScale);
	}
	return texture(texsm, tc);
}
*/
#define inl_sampleShadowMapArray(a,b,c,d,e,f) texture(a, b + vec4(c,0.0,0.0) + (NoiseTap ? vec4(inl_sampleShadowNoise_offset2(d,e,f),0.0,0.0) : vec4(0.0)))

float evalShadowMapArray(in ARG_SAMP_HIGHP sampler2DArrayShadow texsm,
const in vec3 params, const in vec4 position){
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
in vec4 tc, const in vec2 tcnoise, const in mat2x3 tcnoiseMatrix){
	if(NoiseTap){
		tc.stp += tcnoiseMatrix * sampleShadowNoise(tcnoise);
	}
	return texture(texsm, tc);
}

float sampleShadowCube(in ARG_SAMP_HIGHP samplerCubeShadow texsm, in vec4 tc,
const in vec3 tcoffset, const in vec2 tcnoise, const in ivec2 tcnoiseOffset,
const in mat2x3 tcnoiseMatrix){
	tc.stp += tcoffset;
	if(NoiseTap){
		tc.stp += tcnoiseMatrix * sampleShadowNoise(tcnoise, vec2(tcnoiseOffset));
	}
	return texture(texsm, tc);
}

float evalShadowCube(in ARG_SAMP_HIGHP samplerCubeShadow texsm,
const in vec3 params, const in vec4 position){
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
const in vec3 position, const in vec2 depthTransform){
	float thickness = texture(texsm, vec2(position)).r;
	thickness = depthTransform.x / (depthTransform.y - thickness);
	thickness = (depthTransform.x / (depthTransform.y - position.p)) - thickness;
	return max(thickness, 0.0);
}

float shadowMapArrayThickness(in ARG_SAMP_HIGHP sampler2DArray texsm,
const in float scale, const in vec4 position){
	float thickness = texture(texsm, vec3(position)).r * scale;
	thickness = position.q * scale - thickness;
	return max(thickness, 0.0);
}

float shadowCubeThickness(in ARG_SAMP_HIGHP samplerCube texsm,
const in vec4 position, const in vec2 depthTransform){
	float thickness = texture(texsm, vec3(position)).r;
	thickness = thickness * depthTransform.x + depthTransform.y;
	thickness = position.q - thickness;
	return max(thickness, 0.0);
}



// screen space shadows
/////////////////////////

float screenSpaceShadow(ARG_SAMP_HIGHP sampler2DArray samplerDepth,
ARG_SAMP_HIGHP sampler2DArray samplerNormal, const in vec3 position, const in vec3 lightDir){
	float maxLength = pSSShadowMaxLengthBase + position.z * pSSShadowMaxLengthScalePerMeter;
	float thickness = pSSShadowThicknessBase + position.z * pSSShadowThicknessScalePerMeter;
	int stepCount = int(pSSShadowStepCount);
	
	float stepSize = maxLength / float(stepCount);
	vec3 rayDir = lightDir * stepSize;
	vec3 rayPosition = position;
	
	ivec2 tcFrag = ivec2(gl_FragCoord.xy);
	ivec3 tcTest = ivec3(tcFrag, vLayer);
	
	vec3 screenCoord = vec3(0.0, 0.0, vLayer);
	vec3 testPosition;
	
	ivec2 sizeNoise = textureSize(texShadowNoise, 0);
	vec4 noiseValue = texelFetch(texShadowNoise, tcFrag % sizeNoise, 0);
	
	// randomly shift the ray start position. this hides banding of the discrete step size with noise
	rayPosition += rayDir * vec3(noiseValue.x);
	
	for(int i=0; i<stepCount; i++){
		rayPosition += rayDir;
		
		// transform ray position into screen coordinates. if false is returned the ray left the
		// screen and any further testing can be skipped
		if(!positionToScreen(rayPosition, vLayer, screenCoord.xy)){
			break;
		}
		
		// skip if the integer texture coordinates are the same as the original fragment.
		// this happens if the light direction combined with small step size does not move
		// the pixel to sample enough. this avoids fragments testing themselves and considering
		// themselves as blockers which would cause fragments to end up in shadow that should not
		tcTest.xy = ivec2((screenCoord.xy + vec2(1.0)) / vec2(2.0) * pRenderSize);
		if(tcTest.xy == tcFrag){
			continue;
		}
		
		float depth = sampleDepth(samplerDepth, tcTest);
		testPosition = depthToPosition(depth, screenCoord.xy, vLayer);
		
		float diff = rayPosition.z - testPosition.z;
		if(InverseDepth){
			diff = -diff;
		}
		
		// consider in shadow only if the ray position is behind the test position and the
		// ray most probably does not pass behind the blocker. the thickness here is a guess.
		// a better solution would be to stored while rendering the estimated thickness of
		// the geometry. then rays passing behind the blocker could be better detected
		if(diff <= 0.001/*0.0*/ || diff >= thickness){
			continue;
		}
		
		// fade the shadow towards the end of the ray
		float shadow = pow(length(rayPosition - position) / maxLength, 2.0);
		
		// fade the shadow towards the screen border
		vec2 scblend = min((vec2(1.0) - abs(screenCoord.xy)) / vec2(pSSShadowBorderBlendRange), vec2(1.0));
		shadow = mix(1.0, shadow, scblend.x * scblend.y);
		
		return shadow;
	}
	
	return 1.0;
}
