// request high precision if the graphic card supports this
precision HIGHP float;
precision HIGHP int;

// Some helper definitions to make the code easier to read. the strange way of defining
// the tokens is just due to nVidia GLSL compiler failing to support breaking the #if
// condition expression across multiple lines as ATI can. To avoid overly long lines
// a bit of macro-magic is used
#if defined TEXTURE_SHADOW1_SOLID || defined TEXTURE_SHADOW1_TRANSPARENT
	#define _TS1S 1
#endif
#if defined TEXTURE_SHADOW2_SOLID || defined TEXTURE_SHADOW2_TRANSPARENT
	#define _TS2S 1
#endif

#ifndef PARTICLE_LIGHT
	// shadow casting is currently not supported on particle lights. if shadow casting
	// would be enabled shaders fail to compile since PARTICLE_LIGHT prevents the
	// inclusion of various pShader* uniforms in InstanceParameters uniform block
	#if defined _TS1S || defined _TS2S
		#if defined SMA1_CUBE || defined SMA2_CUBE
			#define EVALUATE_SHADOWCUBE 1
		#endif
		#if ! defined SMA1_CUBE || ! defined SMA2_CUBE
			#define EVALUATE_SHADOW2D 1
		#endif
		#define SHADOW_CASTING 1
	#endif
#endif

#if defined TEXTURE_SHADOW1_TRANSPARENT || defined TEXTURE_SHADOW2_TRANSPARENT
	#define COLORED_SHADOWS 1
#endif

#if defined SKY_LIGHT && ! defined GI_RAY
	#define USE_ARRAY_FORM 1
#endif



// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"
#include "shared/defren/light/ubo_light_parameters.glsl"
#include "shared/equirectangular.glsl"



// Samplers
/////////////

#ifdef HW_DEPTH_COMPARE
	#ifdef USE_ARRAY_FORM
		#define SAMPLER_SHADOW2D sampler2DArrayShadow
	#else
		#define SAMPLER_SHADOW2D sampler2DShadow
	#endif
	#define SAMPLER_SHADOWCUBE samplerCubeShadow
#else
	#ifdef USE_ARRAY_FORM
		#define SAMPLER_SHADOW2D sampler2DArray
	#else
		#define SAMPLER_SHADOW2D sampler2D
	#endif
	#define SAMPLER_SHADOWCUBE samplerCube
#endif

#ifdef USE_ARRAY_FORM
	#define SAMPLER_DEPTH2D sampler2DArray
#else
	#define SAMPLER_DEPTH2D sampler2D
#endif

#ifdef GI_RAY
	uniform HIGHP sampler2D texPosition;
	uniform lowp sampler2D texDiffuse;
	uniform lowp sampler2D texNormal;
	uniform lowp sampler2D texReflectivity; // reflectivity.rgb, roughness
#else
	uniform HIGHP sampler2DArray texDepth;
	uniform lowp sampler2DArray texDiffuse;
	uniform lowp sampler2DArray texNormal;
	uniform lowp sampler2DArray texReflectivity;
	uniform lowp sampler2DArray texRoughness;
	uniform lowp sampler2DArray texAOSolidity;
#endif

uniform HIGHP sampler2DArray texSubSurface;

#ifdef SMA1_CUBE
	uniform HIGHP SAMPLER_SHADOWCUBE texShadow1SolidDepth;
	uniform HIGHP SAMPLER_SHADOWCUBE texShadow1TransparentDepth;
	uniform lowp samplerCube texShadow1TransparentColor;
	uniform HIGHP SAMPLER_SHADOWCUBE texShadow1Ambient;
	uniform HIGHP SAMPLER_SHADOWCUBE texShadow2Ambient;
	uniform HIGHP samplerCube texLightDepth1;
#else
	uniform HIGHP SAMPLER_SHADOW2D texShadow1SolidDepth;
	uniform HIGHP SAMPLER_SHADOW2D texShadow1TransparentDepth;
	uniform lowp sampler2D texShadow1TransparentColor;
	uniform HIGHP SAMPLER_SHADOW2D texShadow1Ambient;
	uniform HIGHP SAMPLER_SHADOW2D texShadow2Ambient;
	uniform HIGHP SAMPLER_DEPTH2D texLightDepth1;
#endif

#ifdef SMA2_CUBE
	uniform HIGHP SAMPLER_SHADOWCUBE texShadow2SolidDepth;
	uniform HIGHP SAMPLER_SHADOWCUBE texShadow2TransparentDepth;
	uniform lowp samplerCube texShadow2TransparentColor;
	uniform HIGHP samplerCube texLightDepth2;
#else
	uniform HIGHP SAMPLER_SHADOW2D texShadow2SolidDepth;
	uniform HIGHP SAMPLER_SHADOW2D texShadow2TransparentDepth;
	uniform lowp sampler2D texShadow2TransparentColor;
	uniform HIGHP SAMPLER_DEPTH2D texLightDepth2;
#endif

uniform lowp sampler2D texNoise;

uniform lowp sampler2D texColor;
uniform lowp samplerCube texColorCubemap;
uniform lowp sampler2D texColorEquirect;


// Inputs
///////////

#ifdef FULLSCREENQUAD
	in vec2 vScreenCoord;
#else
	in vec3 vLightVolumePos;
#endif

flat in int vLayer;

#if (defined GS_RENDER_STEREO || defined VS_RENDER_STEREO) && ! defined GI_RAY
	#define vInLayer vLayer
#else
	const int vInLayer = 0;
#endif

#ifdef PARTICLE_LIGHT
	in vec3 vParticleLightPosition;
	in vec3 vParticleLightColor;
	in float vParticleLightRange;
	
	#define vLightPosition vParticleLightPosition
	#define pLightColor vParticleLightColor
	#define pLightRange vParticleLightRange
	
#else
	#define vLightPosition pLightPosition[vInLayer]
#endif



// Outputs
////////////

layout(location=0) out vec4 outColor;
#ifndef GI_RAY
	layout(location=1) out vec4 outLuminance;
		// vec4 not float since blending is required. if float is used
		// GPU randomly writes NaN values breaking rendering
#endif
#ifdef WITH_SUBSURFACE
	layout(location=2) out vec4 outSubSurface;
#endif



// Constants
//////////////

// texDiffuse: color.r, color.g, color.b, alpha
// texNormal: normal.x, normal.y, normal.z
// texReflectivity: reflectivity.r, reflectivity.g, reflectivity.b
// texRoughness: roughness, n/a, n/a
// texAOSolidity: ao, ssao, solidity
// texSubSurface: subsurface.rgb

const float epsilon = 0.0001;

const float shadowThreshold = -0.0174524; // cos of 1 radians
const float shadowThresholdInv = 1.0 / 0.0174524;

#ifdef PCF_4TAP
	const vec4 pcfTCScale = vec4(0.5, 0.5, -0.5, -0.5);
	const float pcfWeight = 0.25;
	
#elif defined PCF_9TAP
	const vec4 pcfTCScale = vec4(1.0, 1.0, -1.0, -1.0);
	const vec4 pcfTCScale2 = vec4(1.0, 1.0, 0.0, 0.0);
	const float pcfWeight = 1.0 / 9.0;
	
#elif defined NOISE_TAP
	const vec2 noiseScale = vec2(1.0);
	const vec2 noiseOffset = vec2(-0.5);
#endif

const vec3 unpackDepth = vec3(1.0, 1.0 / 256.0, 1.0 / 65536.0);

const vec3 ambientLightFactor = vec3(0.25, 0.5, 0.25);

const vec3 lumiFactors = vec3(0.2125, 0.7154, 0.0721);
//const vec3 lumiFactors = vec3(0.3086, 0.6094, 0.0820); // nVidia



// Includes using definitions above
/////////////////////////////////////

// never used and file does not exist. what has it been added for in the first place?
//#include "shared/defren/light/occtracing.glsl"
//#include "shared/defren/light/normal_from_depth.glsl"

#include "shared/normal_texture.glsl"
#include "shared/defren/depth_to_position.glsl"
#include "shared/defren/depth_to_position_fragment.glsl"


// Macros to increase readability and extendibility
/////////////////////////////////////////////////////

#ifdef DECODE_IN_SHADOW
	#define SHATEX(sampler, texcoord) dot(texture(sampler, texcoord).rgb, unpackDepth)
#else
	#define SHATEX(sampler, texcoord) texture(sampler, texcoord)
#endif

#ifdef EVALUATE_SHADOW2D
	#ifdef NOISE_TAP
		#define TAP_NOISE(tc, tcscale) \
			(texture(texNoise, tc).rg * noiseScale + noiseOffset) * tcscale
		#define TAP_NOISE_OFFSET(tc, tcoffset, tcscale) \
			(textureOffset(texNoise, tc, tcoffset).rg * noiseScale + noiseOffset) * tcscale
	#endif
	
	#ifdef USE_ARRAY_FORM
		#ifdef HW_DEPTH_COMPARE
			#ifdef NOISE_TAP
				#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
					SHATEX(texsm, vec4(tc.st + TAP_NOISE(tcnoise, tcnoiseScale), tc.pq))
				#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
					SHATEX(texsm, vec4(tc.st + tcoffset + TAP_NOISE_OFFSET(tcnoise, tcnoiseOffset, tcnoiseScale), tc.pq))
			#else
				#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
					SHATEX(texsm, tc)
				#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
					SHATEX(texsm, vec4(tc.st + tcoffset, tc.pq))
			#endif
		#else
			#ifdef NOISE_TAP
				#ifdef SHADOW_INVERSE_DEPTH
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(SHATEX(texsm, vec3(tc.st + TAP_NOISE(tcnoise, tcnoiseScale), tc.p)), tc.q)
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(SHATEX(texsm, vec3(tc.st + tcoffset + TAP_NOISE_OFFSET(tcnoise, tcnoiseOffset, tcnoiseScale), tc.p)), tc.q)
				#else
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(tc.q, SHATEX(texsm, vec3(tc.st + TAP_NOISE(tcnoise, tcnoiseScale), tc.p)))
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(tc.q, SHATEX(texsm, vec3(tc.st + tcoffset + TAP_NOISE_OFFSET(tcnoise, tcnoiseOffset, tcnoiseScale), tc.p)))
				#endif
			#else
				#ifdef SHADOW_INVERSE_DEPTH
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(SHATEX(texsm, tc.stp), tc.q)
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(SHATEX(texsm, vec3(tc.st + tcoffset, tc.p)), tc.q)
				#else
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(tc.q, SHATEX(texsm, tc.stp))
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(tc.q, SHATEX(texsm, vec3(tc.st + tcoffset, tc.p)))
				#endif
			#endif
		#endif
	#else
		#ifdef HW_DEPTH_COMPARE
			#ifdef NOISE_TAP
				#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
					SHATEX(texsm, vec3(tc.st + TAP_NOISE(tcnoise, tcnoiseScale), tc.p))
				#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
					SHATEX(texsm, vec3(tc.st + tcoffset + TAP_NOISE_OFFSET(tcnoise, tcnoiseOffset, tcnoiseScale), tc.p))
			#else
				#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
					SHATEX(texsm, tc)
				#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
					SHATEX(texsm, vec3(tc.st + tcoffset, tc.p))
			#endif
		#else
			#ifdef NOISE_TAP
				#ifdef SHADOW_INVERSE_DEPTH
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(SHATEX(texsm, tc.st + TAP_NOISE(tcnoise, tcnoiseScale)), tc.p)
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(SHATEX(texsm, tc.st + tcoffset + TAP_NOISE_OFFSET(tcnoise, tcnoiseOffset, tcnoiseScale)), tc.p)
				#else
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(tc.p, SHATEX(texsm, tc.st + TAP_NOISE(tcnoise, tcnoiseScale)))
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(tc.p, SHATEX(texsm, tc.st + tcoffset + TAP_NOISE_OFFSET(tcnoise, tcnoiseOffset, tcnoiseScale)))
				#endif
			#else
				#ifdef SHADOW_INVERSE_DEPTH
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(SHATEX(texsm, tc.st), tc.p)
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(SHATEX(texsm, tc.st + tcoffset), tc.p)
				#else
					#define TAP_DEPTH2D(texsm, tc, tcnoise, tcnoiseScale) \
						step(tc.p, SHATEX(texsm, tc.st))
					#define TAP_DEPTH2D_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseScale) \
						step(tc.p, SHATEX(texsm, tc.st + tcoffset))
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef EVALUATE_SHADOWCUBE
	#ifdef NOISE_TAP
		#define TAP_NOISE2(tc) \
			(texture(texNoise, tc).rg * noiseScale + noiseOffset)
		#define TAP_NOISE_OFFSET2(tc, tcoffset) \
			(textureOffset(texNoise, tc, tcoffset).rg * noiseScale + noiseOffset)
	#endif
	
	#ifdef HW_DEPTH_COMPARE
		#ifdef NOISE_TAP
			#define TAP_DEPTHCUBE(texsm, tc, tcnoise, tcnoiseMatrix) \
				SHATEX(texsm, vec4(tc.stp + tcnoiseMatrix * TAP_NOISE2(tcnoise), tc.q))
			#define TAP_DEPTHCUBE_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseMatrix) \
				SHATEX(texsm, vec4(tc.stp + tcoffset + tcnoiseMatrix * TAP_NOISE_OFFSET2(tcnoise, tcnoiseOffset), tc.q))
		#else
			#define TAP_DEPTHCUBE(texsm, tc, tcnoise, tcnoiseMatrix) \
				SHATEX(texsm, tc)
			#define TAP_DEPTHCUBE_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseMatrix, tcnoiseOffset) \
				SHATEX(texsm, vec4(tc.stp + tcoffset, tc.q))
		#endif
	#else
		#ifdef NOISE_TAP
			#ifdef SHADOW_INVERSE_DEPTH
				#define TAP_DEPTHCUBE(texsm, tc, tcnoise, tcnoiseMatrix) \
					step(SHATEX(texsm, tc.stp + tcnoiseMatrix * TAP_NOISE2(tcnoise)), tc.q)
				#define TAP_DEPTHCUBE_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseMatrix) \
					step(SHATEX(texsm, tc.stp + tcoffset + tcnoiseMatrix * TAP_NOISE_OFFSET2(tcnoise, tcnoiseOffset)), tc.q)
			#else
				#define TAP_DEPTHCUBE(texsm, tc, tcnoise, tcnoiseMatrix) \
					step(tc.q, SHATEX(texsm, tc.stp + tcnoiseMatrix * TAP_NOISE2(tcnoise)))
				#define TAP_DEPTHCUBE_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseOffset, tcnoiseMatrix) \
					step(tc.q, SHATEX(texsm, tc.stp + tcoffset + tcnoiseMatrix * TAP_NOISE_OFFSET2(tcnoise, tcnoiseOffset)))
			#endif
		#else
			#ifdef SHADOW_INVERSE_DEPTH
				#define TAP_DEPTHCUBE(texsm, tc, tcnoise, tcnoiseMatrix) \
					step(SHATEX(texsm, tc.stp), tc.q)
				#define TAP_DEPTHCUBE_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseMatrix, tcnoiseOffset) \
					step(SHATEX(texsm, tc.stp + tcoffset), tc.q)
			#else
				#define TAP_DEPTHCUBE(texsm, tc, tcnoise, tcnoiseMatrix) \
					step(SHATEX(texsm, tc.stp), tc.q)
				#define TAP_DEPTHCUBE_OFFSET(texsm, tc, tcoffset, tcnoise, tcnoiseMatrix, tcnoiseOffset) \
					step(SHATEX(texsm, tc.stp + tcoffset), tc.q)
			#endif
		#endif
	#endif
#endif



// Evaluate 2D shadow map
///////////////////////////

#ifdef EVALUATE_SHADOW2D
#ifdef USE_ARRAY_FORM
	#define ES2DTC vec4
	#define ES2D(tc) tc
#else
	#define ES2DTC vec3
	#define ES2D(tc) (tc).stp
#endif

// for HW_DEPTH_COMPARE mediump is required
float evaluateShadow2D(in ARG_SAMP_HIGHP SAMPLER_SHADOW2D texsm, in vec3 params, in ES2DTC position){
	vec4 tcoffset;
	float shadow;
	
	#ifdef NOISE_TAP
		vec2 tcnoise = gl_FragCoord.xy / vec2(32.0);
	#endif
	
	#ifdef PCF_VARTAP
		//vec2 shascale = params.xy * vec2(clamp(position.p * 5.0, 0.5, 5.0));
		// problem is that position.q is perspective in the shadow map. for the var-tap though
		// the distance would be required. just multiplying by position.q doesn't work since
		// then it's relative to the scaling. besides q is not transported in the position
		// unless required for special use
		ivec2 vtcount = ivec2(-2, 2);
		float vtweight = 25.0; // 5 pixel * 5 pixel
		ES2DTC tc = position;
		int x, y;
		
		shadow = 0.0;
		
		for(y=vtcount.x; y<vtcount.y; y++){
			tc.t = shascale.y * float(y) + position.t;
			
			for(x=vtcount.x; x<vtcount.y; x++){
				tc.s = shascale.x * float(x) + position.s;
				shadow += TAP_DEPTH2D(texsm, tc, tcnoise, params.zz);
			}
		}
		
		shadow /= vtweight;
		
	#elif defined PCF_9TAP
		shadow = TAP_DEPTH2D(texsm, position, tcnoise, params.zz);
		
	#elif defined PCF_4TAP
		shadow = 0.0;
		
	#else
		shadow = TAP_DEPTH2D(texsm, position, tcnoise, params.zz);
	#endif
	
	#if defined PCF_4TAP || defined PCF_9TAP
		tcoffset = params.xyxy * pcfTCScale; // (u,v) (u,-v) (-u,v) (-u,-v) . with PCF_4TAP u/2 and v/2
		shadow += TAP_DEPTH2D_OFFSET(texsm, position, tcoffset.xy, tcnoise, ivec2(1,1), params.zz);
		shadow += TAP_DEPTH2D_OFFSET(texsm, position, tcoffset.xw, tcnoise, ivec2(1,-1), params.zz);
		shadow += TAP_DEPTH2D_OFFSET(texsm, position, tcoffset.zy, tcnoise, ivec2(-1,1), params.zz);
		shadow += TAP_DEPTH2D_OFFSET(texsm, position, tcoffset.zw, tcnoise, ivec2(-1,-1), params.zz);
	#endif
	
	#ifdef PCF_9TAP
		tcoffset = params.xyxy * pcfTCScale2; // (u, 0) (-u, 0) (0, v) (0, -v)
		shadow += TAP_DEPTH2D_OFFSET(texsm, position,  tcoffset.xw, tcnoise, ivec2(1,0), params.zz);
		shadow += TAP_DEPTH2D_OFFSET(texsm, position, -tcoffset.xw, tcnoise, ivec2(-1,0), params.zz);
		shadow += TAP_DEPTH2D_OFFSET(texsm, position,  tcoffset.zy, tcnoise, ivec2(0,1), params.zz);
		shadow += TAP_DEPTH2D_OFFSET(texsm, position, -tcoffset.zy, tcnoise, ivec2(0,-1), params.zz);
	#endif
	
	#if defined PCF_4TAP || defined PCF_9TAP
		shadow *= pcfWeight;
	#endif
	
	return shadow;
}
#endif



// Evaluate shadow cube map
/////////////////////////////

#ifdef EVALUATE_SHADOWCUBE
float evaluateShadowCube(in ARG_SAMP_HIGHP SAMPLER_SHADOWCUBE texsm, in vec3 params, in vec4 position){
	float shadow;
	
	#ifdef NOISE_TAP
		vec2 tcnoise = gl_FragCoord.xy / vec2(32.0);
	#endif
	
	#if defined PCF_9TAP || defined PCF_4TAP || defined PCF_VARTAP || defined PCF_NOISETAP
		float pdist = length(position.xyz);
		float nolen = pdist * params.z;
		vec3 no1, no2;
		
		vec3 aposition = abs(position.xyz);
		bvec4 atest = greaterThan(aposition.xxyy, aposition.yzxz); // (x>y, x>z, y>x, y>z)
		//vec3 r = step(absshapos.xyz, absshapos.yzx) * step(absshapos.xyz, absshapos.zxy);
		//vec2 scale = vec2(dot(absshapos, r) * pShaOpts1.z);
		//o1.xz = (c2 * r.xx + c1) * scale;
		//o2.zy = (c2 * r.yy + c1) * scale;
		if(all(atest.xy)){ // x is largest
			no1 = vec3(0.0, 0.0, nolen);
			no2 = vec3(0.0, nolen, 0.0);
			
		}else if(all(atest.zw)){ // y is largest
			no1 = vec3(nolen, 0.0, 0.0);
			no2 = vec3(0.0, 0.0, nolen);
			
		}else{ // z is largest
			no1 = vec3(nolen, 0.0, 0.0);
			no2 = vec3(0.0, nolen, 0.0);
		}
		
		mat2x3 noiseMatrix = mat2x3(no1, no2);
	#endif
	
	#ifdef PCF_VARTAP
		vec2 shascale = params.xy * vec2(clamp(pdist * 5.0, 0.5, 5.0));
		vec4 tc = position;
		vec3 offy;
		int x, y;
		
		no1 *= vec3(shascale.x);
		no2 *= vec3(shascale.y);
		noiseMatrix = mat2x3(no1, no2); //noiseMatrix *= shascale;
		shadow = 0.0;
		
		for(y=-2; y<2; y++){
			offy = position.stp + no2 * vec3(y);
			
			for(x=-2; x<2; x++){
				tc.xyz = offy + no1 * vec3(x);
				shadow += TAP_DEPTHCUBE(texsm, tc, tcnoise, noiseMatrix);
			}
		}
		
		shadow /= 25.0;
		
	#elif defined PCF_9TAP
		shadow = TAP_DEPTHCUBE(texsm, position, tcnoise, noiseMatrix);
		
	#elif defined(PCF_4TAP)
		shadow = 0.0;
		
	#else
		shadow = TAP_DEPTHCUBE(texsm, position, tcnoise, noiseMatrix);
	#endif
	
	#if defined PCF_4TAP || defined PCF_9TAP
		// (u,v) (u,-v) (-u,v) (-u,-v) . with PCF_4TAP u/2 and v/2
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * pcfTCScale.xy, tcnoise, ivec2(1,1), noiseMatrix);
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * pcfTCScale.xw, tcnoise, ivec2(1,-1), noiseMatrix);
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * pcfTCScale.zy, tcnoise, ivec2(-1,1), noiseMatrix);
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * pcfTCScale.zw, tcnoise, ivec2(-1,-1), noiseMatrix);
	#endif
	
	#ifdef PCF_9TAP
		// (u, 0) (-u, 0) (0, v) (0, -v)
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * pcfTCScale2.xw, tcnoise, ivec2(1,0), noiseMatrix);
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * -pcfTCScale2.xw, tcnoise, ivec2(-1,0), noiseMatrix);
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * pcfTCScale2.zy, tcnoise, ivec2(0,1), noiseMatrix);
		shadow += TAP_DEPTHCUBE_OFFSET(texsm, position, noiseMatrix * -pcfTCScale2.zy, tcnoise, ivec2(0,-1), noiseMatrix);
	#endif
	
	#if defined PCF_4TAP || defined PCF_9TAP
		shadow *= pcfWeight;
	#endif
	
	return shadow;
}
#endif



// Retrieve shadow map depth
///////////////////////////////

#ifdef WITH_SUBSURFACE
	#ifdef DECODE_IN_SHADOW
		#define SSSHATEX(sampler, texcoord) dot(texture(sampler, texcoord).rgb, unpackDepth)
	#else
		#define SSSHATEX(sampler, texcoord) texture(sampler, texcoord).r
	#endif
	
	// distanceBlocker2Fragment = distanceFragmentToLight * (1 - depthBlocker / depthFragment)
	// this works only with linear shadow maps. with projective shadow maps the depths have to
	// be first back-transformed into distances where a simple subtraction can then be done
	
	#ifdef EVALUATE_SHADOW2D
		#ifdef USE_ARRAY_FORM
			float thicknessShadow2DArray(in ARG_SAMP_HIGHP sampler2DArray texsm, in float scale, in vec4 position){
				float thickness = SSSHATEX(texsm, position.stp) * scale;
				thickness = position.q * scale - thickness;
				
				//return max(thickness, 0.0);
				return thickness;
			}
		#else
			float thicknessShadow2D(in ARG_SAMP_HIGHP sampler2D texsm, in vec3 position){
				float thickness = SSSHATEX(texsm, position.st);
				thickness = pShadowDepthTransform.z / (pShadowDepthTransform.w - thickness);
				thickness = (pShadowDepthTransform.z / (pShadowDepthTransform.w - position.p)) - thickness;
				
				//return max(thickness, 0.0);
				return thickness;
			}
		#endif
	#endif
	
	#ifdef EVALUATE_SHADOWCUBE
	float thicknessShadowCube(in ARG_SAMP_HIGHP samplerCube texsm, in vec4 position){
		float thickness = SSSHATEX(texsm, position.stp);
		thickness = thickness * pShadowDepthTransform.z + pShadowDepthTransform.w;
		thickness = position.q - thickness;
		
		//return max(thickness, 0.0);
		return thickness;
	}
	#endif
#endif



// Discard fragment without disabling early-z
///////////////////////////////////////////////

void outputUnlit(){
	outColor = vec4(0);
	#ifndef GI_RAY
		outLuminance = vec4(0);
	#endif
	#ifdef WITH_SUBSURFACE
		outSubSurface = vec4(0);
	#endif
}



// Main Function
//////////////////

void main(void){
	ivec3 tcArray = ivec3(gl_FragCoord.xy, vInLayer);
	#ifdef LUMINANCE_ONLY
		tcArray.xy *= pLumFragCoordScale;
	#else
	
	#ifdef GI_RAY
		ivec2 tc = ivec2(tcArray);
	#else
		ivec3 tc = tcArray;
	#endif
	
	// discard not inizalized fragments or fragements that are not supposed to be lit
	vec4 diffuse = texelFetch(texDiffuse, tc, 0);
	#ifndef GI_RAY
		if(diffuse.a == 0.0){
			outputUnlit();
			return;
		}
	#endif
	
	// determine position of fragment to light
	#ifdef GI_RAY
		vec4 positionDistance = texelFetch(texPosition, tc, 0);
		if(positionDistance.a > 9999.0){
			// ray hits nothing. for sky lights this adds the ambient light contribution.
			// for all other light sources do not light the ray
			#ifdef SKY_LIGHT
				vec3 lightColor = pLightColorAmbientGI;
				//outLuminance = vec4(vec3(dot(lightColor, lumiFactors)), diffuse.a);
				outColor = vec4(lightColor * diffuse.rgb, diffuse.a);
			#else
				outputUnlit();
			#endif
			return;
		}
		vec3 position = vec3(pGIRayMatrix * vec4(positionDistance.rgb, 1.0));
	#else
		float depth = sampleDepth(texDepth, tc);
		#ifdef FULLSCREENQUAD
			vec3 position = depthToPosition(depth, vScreenCoord, vInLayer);
		#else
			vec3 position = depthToPositionVolume(depth, vLightVolumePos, vInLayer);
		#endif
	#endif
	
	// calculate light direction and distance
	#ifdef SKY_LIGHT
		#define lightDir pLightView[vInLayer]
	#else
		vec3 lightDir = vLightPosition - position;
		float dist = length(lightDir);
		
		// discard if pre-lit (length = 0) or outside the light range
		if(dist == 0.0 || dist > pLightRange){
			outputUnlit();
			return;
		}
		lightDir /= dist;
		
		// discard fragments outside the spot cone
		// NOTE this does not work anymore since spot cone can be squashed now
		/*
		#if defined SPOT_LIGHT
			float spotFactor = dot(lightView, -lightDir) - pLightSpotBase;
			if(spotFactor <= 0.0){
				discard;
			}
		#endif
		*/
	#endif
	
	// calculate shadow position if required
	#ifdef SKY_LIGHT
		#ifdef SHADOW_CASTING
			#ifdef WITH_SUBSURFACE
			float thicknessShadowScale;
			#endif
			vec4 shapos1;
			
			if(position.z < pLayerBorder.x){
				shapos1 = (pShadowMatrix1[vInLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 0.0; // layer 0
				#ifdef WITH_SUBSURFACE
				thicknessShadowScale = pShadowDepthTransform.z;
				#endif
				
			}else if(position.z < pLayerBorder.y){
				shapos1 = (pShadowMatrix2[vInLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 1.0; // layer 1
				#ifdef WITH_SUBSURFACE
				thicknessShadowScale = pShadowDepthTransform.w;
				#endif
				
			}else if(position.z < pLayerBorder.z){
				shapos1 = (pShadowMatrix3[vInLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 2.0; // layer 2
				#ifdef WITH_SUBSURFACE
				thicknessShadowScale = pShadowDepthTransform2.z;
				#endif
				
			}else{
				shapos1 = (pShadowMatrix4[vInLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 3.0; // layer 3
				#ifdef WITH_SUBSURFACE
				thicknessShadowScale = pShadowDepthTransform2.w;
				#endif
			}
			
			#define shapos2 shapos1
		#endif
		
	#elif defined PARTICLE_LIGHT
		vec4 shapos1 = vec4(position - vParticleLightPosition, 1);
		
	#else
		vec4 shapos1 = pShadowMatrix1[vInLayer] * vec4(position, 1);
		
		#ifdef SMA1_CUBE
			vec3 absshapos1 = abs(vec3(shapos1));
			float shaZ1 = max(max(absshapos1.x, absshapos1.y), max(absshapos1.z, pLightShadowClamp));
			shapos1.q = (shaZ1 * pShadowDepthTransform.x + pShadowDepthTransform.y) / shaZ1;
			
		#else
			shapos1.pq = max(shapos1.pq, vec2(pLightShadowClamp));
			shapos1.stp /= shapos1.q;
		#endif
		
		#ifdef SPOT_LIGHT
			// discard fragments outside the spot cone. since spot cones can now be squashed
			// the shadow matrix texture coordinates have to be used to get a correct result.
			// this boils down to a circle test in the range 0..1 which is a simple check if
			// the point is more than 0.5 units away from the center. this is always correct
			// the same no matter how squashed the cone is
			float spotConeRadius = length(shapos1.st - vec2(0.5));
			if(spotConeRadius > 0.5){
				outputUnlit();
				return;
			}
			
		#elif defined PROJECTOR_LIGHT
			// discard fragments outide the projector frustum
			vec2 spotConeRadius = abs(shapos1.st - vec2(0.5));
			if(any(greaterThan(spotConeRadius, vec2(0.5)))){
				outputUnlit();
				return;
			}
		#endif
		
		#if defined SHADOW_CASTING && defined TEXTURE_SHADOW2_SOLID
			#ifdef SHAMAT2_EQUALS_SHAMAT1
				#define shapos2 shapos1
			#else
				vec4 shapos2 = pShadowMatrix2[vInLayer] * vec4(position, 1.0);
				
				#ifdef SMA2_CUBE
					//shapos2.w = length(shapos2.xyz);
					vec3 absshapos2 = abs(shapos2.xyz);
					float shaZ2 = max(max(absshapos2.x, absshapos2.y), max(absshapos2.z, pLightShadowClamp));
					shapos2.q = (shaZ2 * pShadowDepthTransform.x + pShadowDepthTransform.y) / shaZ2;
				#else
					shapos2.pq = max(shapos2.pq, vec2(pLightShadowClamp));
					shapos2.stp /= shapos2.q;
				#endif
			#endif
		#endif
	#endif
	
	// fetch normal
	vec3 normal = normalLoadMaterial(texNormal, tc);
	
	#ifdef GI_RAY
		normal = normal * pGIRayMatrixNormal; // requires matrix transpose done by reversed order
	#endif
	
	if(dot(normal, normal) < 0.0001){
		normal = lightDir; // 0-normal means always point towards light source
		
	}else{
		normal = clamp(normal, vec3(-1.0), vec3(1.0)); // some shader writes broken normals (or missing clear?). temporary fix
		normal = normalize(normal);
	}
	
	// calculate the sss thickness from the shadow map if existing
	#ifdef WITH_SUBSURFACE
		vec3 absorptionRadius = texelFetch(texSubSurface, tcArray, 0).rgb;
		float largestAbsorptionRadius = max(max(absorptionRadius.x, absorptionRadius.y), absorptionRadius.z);
		#ifdef SHADOW_CASTING
			#ifdef TEXTURE_SHADOW1_SOLID
				#ifdef SMA1_CUBE
					float shadowThickness = max(0.0, thicknessShadowCube(texLightDepth1, shapos1));
				#else
					#ifdef USE_ARRAY_FORM
						float shadowThickness = max(0.0, thicknessShadow2DArray(texLightDepth1, thicknessShadowScale, shapos1));
					#else
						float shadowThickness = max(0.0, thicknessShadow2D(texLightDepth1, shapos1.stp));
					#endif
				#endif
			#else
				float shadowThickness = 100.0; // just something large enough
			#endif
			
			#ifdef TEXTURE_SHADOW2_SOLID
				#ifdef SMA2_CUBE
					shadowThickness = max(shadowThickness, thicknessShadowCube(texLightDepth2, shapos2));
				#else
					#ifdef USE_ARRAY_FORM
						shadowThickness = max(shadowThickness, thicknessShadow2DArray(texLightDepth2, thicknessShadowScale, shapos2));
					#else
						shadowThickness = max(shadowThickness, thicknessShadow2D(texLightDepth2, shapos2.stp));
					#endif
				#endif
			#endif
		#else
			float shadowThickness = 100.0; // just something large enough
		#endif
	#endif
	
	// calculate shadow color of required
	// NOTE the clamp() call is unfortunately mandatory. small imprecisions in GPU calculation
	//      can cause dot() to output a value which is slightly outside the range totally
	//      upsetting the upcoming calculations
	float dotval = clamp(dot(normal, lightDir), -1.0, 1.0);
	#ifdef WITH_SUBSURFACE
		float absorptionDot = max(-dotval, 0.0);
		
		float absorptionEdgeHack = abs(dotval);
		absorptionEdgeHack *= absorptionEdgeHack;
		absorptionEdgeHack *= absorptionEdgeHack; // pow(1-abs(dot), 4)
		absorptionDot *= absorptionEdgeHack;
	#endif
	
	#ifdef SHADOW_CASTING
		// calculate the solid shadow value. we use a little trick to avoid shadow artifacts as well as
		// avoiding taping the shadow map for pixels it is not required. for this the observation is made
		// that pixels facing away from the light are always in shadows. in this case there is no need
		// for taping the shadow map since the result should be 0 in the correct case.
		float shadow;
		#ifdef OPTIMIZE_SHADOW_BACKFACE
		if(dotval <= shadowThreshold){
			#ifdef AMBIENT_LIGHTING
				shadow = 0.0;
			#else
				#ifdef WITH_SUBSURFACE
				if(shadowThickness > largestAbsorptionRadius){
				#endif
				outputUnlit();
				return;
				#ifdef WITH_SUBSURFACE
				}
				#endif
			#endif
			
		}else{
			shadow = clamp(dotval * shadowThresholdInv + 1, 0, 1);
		#else
			// force back facing fragments into shadow. not only does this avoid the need to
			// sample the shadow maps but it also avoids light leaking problems. this does
			// not affect ambient shadows
			if(dotval > 0.0){
				shadow = 1.0;
		#endif
			#ifdef TEXTURE_SHADOW1_SOLID
				#ifdef SMA1_CUBE
					shadow = min(shadow, evaluateShadowCube(texShadow1SolidDepth, pShadow1Solid, shapos1));
				#elif defined GI_RAY && defined SKY_LIGHT
					// the main shadow map array is only guaranteed to hold valid data for
					// pixels falling into the view frustum. outside this volume data is
					// potentially culled for performance reason. use GI shadow map instead
					// 
					// NOTE this is not working reliably. most probably the view based optimization
					//      is not working properly using the GI view. this causes rays to switch
					//      between receiving sky light or not when slightly rotating the view.
					//      the problematic hits are located behind the camera. for this reason
					//      the GI-ShadowMap is always used although the quality is inferior than
					//      using the view shadow map
// 					vec4 projPos = pMatrixP[0] * vec4(position, 1.0);
// 					if(any(greaterThan(abs(projPos.xyz), vec3(projPos.w)))){
						//shadow = min(shadow, SHATEX(texShadow1SolidDepth, (pGIShadowMatrix * vec4(position, 1.0)).stp));
						vec4 gishapos = pGIShadowMatrix * vec4(position, 1.0);
						shadow = min(shadow, evaluateShadow2D(texShadow1SolidDepth, pGIShadowParams, ES2D(gishapos)));
						
						// prevent the test depth from reaching 0 or below. if this happens the test
						// result incorrectly considers the fragment not in shadows even if it is.
						// it is not enough to clamp to 0. it has to be larger than 0.
// 						shapos1.q = max(shapos1.q, 0.0001);
						
// 					}else{
// 						shadow = min(shadow, evaluateShadow2D(texShadow1SolidDepth, pShadow1Solid, ES2D(shapos1)));
// 					}
				#else
					shadow = min(shadow, evaluateShadow2D(texShadow1SolidDepth, pShadow1Solid, ES2D(shapos1)));
				#endif
			#endif
			#ifdef TEXTURE_SHADOW2_SOLID
				#ifdef SMA2_CUBE
					shadow = min(shadow, evaluateShadowCube(texShadow2SolidDepth, pShadow2Solid, shapos2));
				#elif defined GI_RAY && defined SKY_LIGHT
					shadow = min(shadow, evaluateShadow2D(texShadow2SolidDepth, pGIShadowParams, ES2D(gishapos)));
				#else
					shadow = min(shadow, evaluateShadow2D(texShadow2SolidDepth, pShadow2Solid, ES2D(shapos2)));
				#endif
			#endif
			}else{
				shadow = 0.0;
			}
			
			#ifdef AMBIENT_LIGHTING
				// temporary until sky light is improved. required since pLightAmbientRatio is
				// not defined for sky light which would result in strange errors if this not
				// initialized value is used
				vec3 fullShadowColor = vec3(shadow); // required for specular reflection
				
			#elif defined TEXTURE_SHADOW1_AMBIENT || defined TEXTURE_SHADOW2_AMBIENT
				// if shadow is 1 the fragment is in plain light and has to stay at 1
				// if shadow is 0 the fragment is in shadows. in this case it can received ambient light or not.
				// if ambient light applies it is applied using the ambient ratio parameter of the light source.
				// to avoid problems with light leaking through inside rooms having only a back facing wall the
				// ambient map is used like an additional shadow map before calculating the real result
				float ambientShadow = 1.0;
				#ifdef TEXTURE_SHADOW1_AMBIENT
					#ifdef SMA1_CUBE
						ambientShadow = min(ambientShadow, evaluateShadowCube(texShadow1Ambient, pShadow1Solid, shapos1));
					#else
						ambientShadow = min(ambientShadow, evaluateShadow2D(texShadow1Ambient, pShadow1Solid, ES2D(shapos1)));
					#endif
				#endif
				#ifdef TEXTURE_SHADOW2_AMBIENT
					#ifdef SMA2_CUBE
						ambientShadow = min(ambientShadow, evaluateShadowCube(texShadow2Ambient, pShadow2Solid, shapos2));
					#else
						ambientShadow = min(ambientShadow, evaluateShadow2D(texShadow2Ambient, pShadow2Solid, ES2D(shapos2)));
					#endif
				#endif
				
				vec3 fullShadowColor = vec3(shadow * ambientShadow); // required for specular reflection
				shadow = mix(pLightAmbientRatio, 1.0, shadow) * ambientShadow;
				/*
				#ifdef SMA1_CUBE
					shadow = mix(pLightAmbientRatio * evaluateShadowCube(texShadowAmbient, pShadow1Solid, shapos1), 1.0, shadow);
				#else
					shadow = mix(pLightAmbientRatio * evaluateShadow2D(texShadowAmbient, pShadow1Solid, ES2D(shapos1)), 1.0, shadow);
				#endif
				*/
			#else
				vec3 fullShadowColor = vec3(shadow); // required for specular reflection
				shadow = mix(pLightAmbientRatio, 1.0, shadow);
			#endif
		#ifdef OPTIMIZE_SHADOW_BACKFACE
		}
		#endif
		
		// discard the fragment if it is fully in shadows and there is no ambient light
		#ifndef AMBIENT_LIGHTING
			if(shadow < epsilon){
				#ifdef WITH_SUBSURFACE
				if(shadowThickness > largestAbsorptionRadius){
				#endif
				outputUnlit();
				return;
				#ifdef WITH_SUBSURFACE
				}
				#endif
			}
		#endif
		
		#if GI_RAY
		shadow = step(0.75, shadow);
		#endif
		
		vec3 shadowColor = vec3(shadow);
		
		// calculate the transparent shadow value
		#ifdef COLORED_SHADOWS
			// only evaluate transparency if not in solid shadow
			if(shadow > epsilon){
				float transpShadow;
				vec4 transpColor;
				
				#ifdef TEXTURE_SHADOW1_TRANSPARENT
					#ifdef SMA1_CUBE
						transpShadow = evaluateShadowCube(texShadow1TransparentDepth, pShadow1Transparent, shapos1);
						transpColor = textureLod(texShadow1TransparentColor, shapos1.stp, 0.0);
					#else
						transpShadow = evaluateShadow2D(texShadow1TransparentDepth, pShadow1Transparent, ES2D(shapos1));
						transpColor = textureLod(texShadow1TransparentColor, shapos1.st, 0.0);
					#endif
					shadowColor *= mix(vec3(1.0), transpColor.rgb, vec3((1.0 - transpShadow) * transpColor.a));
				#endif
				#ifdef TEXTURE_SHADOW2_TRANSPARENT
					#ifdef SMA2_CUBE
						transpShadow = evaluateShadowCube(texShadow2TransparentDepth, pShadow2Transparent, shapos2);
						transpColor = textureLod(texShadow2TransparentColor, shapos2.stp, 0.0);
					#else
						transpShadow = evaluateShadow2D(texShadow2TransparentDepth, pShadow2Transparent, ES2D(shapos2));
						transpColor = textureLod(texShadow2TransparentColor, shapos2.st, 0.0);
					#endif
					shadowColor *= mix(vec3(1.0), transpColor.rgb, vec3((1.0 - transpShadow) * transpColor.a));
				#endif
			}
		#endif
	#endif
	
	
	
	// diffuse term = color_diff * color_light * max(dot(normal, lightDir), 0)
	// specular term = ((ap + 2) / 8) * pow(max(dot(normal, halfDir), 0), ap)
	//                   * fresnel(color_reflectivity, lightDir, halfDir)
	//                   * color_light * max(dot(normal, lightDir), 0)
	// 
	// vec3 halfDir = normalize(lightDir - position);
	// vec3 lightTerm = colorLight * vec3(max(dot(normal, lightDir), 0.0));
	// float specNormTerm = (ap + 2.0) / 8.0;
	// float specPowTerm = pow(max(dot(normal, halfDir), 0.0), ap);
	// vec3 specFresnelTerm = mix(colorSpecular, vec3(1.0), vec3(pow(1.0 - dot(lightDir, halfDir), 5.0)));
	// vec3 specularTerm = vec3(specNormTerm * specPowTerm) * specFresnelTerm;
	// vec3 fragmentColor = (colorDiffuse + specularTerm) * lightTerm;
	// 
	// calculation of the ap term using a (pseudo) linear roughness value. roughness value is 0 for sharp reflection
	// all the way to 1 for fully diffuse reflection. roughness is considered to be 1 minus the beckmann roughness.
	// the original formula is ap = 2 / roughness**2 - 2. to avoid a division by zero a small max ap factor is used.
	// this factor clamps the ap value to a given maximum value. a value of 0.00002 for the max ap factor clamps the
	// ap to roughly 100000. this is enough for a fully sharp reflection. to avoid a near flat line result at the
	// fully diffuse end of the ap spectrum a min ap factor is used. the smallest physically reasonable ap values
	// are in the range from 0.1 to 2. a min ap factor of 2-0.5 = 1.5 keeps the smallest ap to 0.5 which looks nice.
	// 
	// roughness = clamp(texture-input(0=sharp .. 1=diffuse), 0.0, 1.0)
	// float ap = 2.0 / (specularity.g * specularity.g + maxAP) - minAP
	#ifdef GI_RAY
		vec4 reflRough = texelFetch(texReflectivity, tc, 0);
		vec3 reflectivity = reflRough.rgb;
		vec3 roughness = vec3(reflRough.a, 0.0, 0.0);
		
	#else
		vec3 reflectivity = texelFetch(texReflectivity, tc, 0).rgb;
		vec3 roughness = texelFetch(texRoughness, tc, 0).rgb;
		vec3 aoSolidity = texelFetch(texAOSolidity, tc, 0).rgb;
	#endif
	
	// merge the texture-ao with the ssao. use the minimum of the two to avoid over-occluding if both are used.
	// the result is stored in aoSolidity.g . this way aoSolidity.r contains the pure texture-ao and
	// aoSolidity.gb the combined ao
	#ifndef GI_RAY
		aoSolidity.g = min(aoSolidity.r, aoSolidity.g);
		diffuse.a *= aoSolidity.b;
	#endif
	
	// specular term
	//roughness.r = max(roughness.r, 0.1);
	float ap = 426.0 * (1.0 - roughness.r) / (90.0 * roughness.r * roughness.r + roughness.r + 0.001) + 1.0;
	vec3 halfDir = normalize(lightDir - normalize(position));
	float specNormTerm = (ap + 2.0) / 8.0;
	float specPowTerm = pow(clamp(dot(normal, halfDir), 0.0, 0.99), ap); // 0.99 prevents infinity overshoot on near 0-angle ray
	vec3 specFresnelTerm = mix(reflectivity, vec3(1.0), vec3(pow(clamp(1.0 - dot(lightDir, halfDir), 0.0, 1.0), 5.0)));
	
	#ifdef AMBIENT_LIGHTING
		//float aldotval = dot(vec3(dotval * dotval, dotval, 1.0), ambientLightFactor);
	#endif
	dotval = max(dotval, 0.0);
	
	#ifdef SHADOW_CASTING
	specFresnelTerm *= fullShadowColor; // no specular reflection in shadows
	#endif
	vec3 finalColorSurface = clamp(vec3(specNormTerm * specPowTerm) * specFresnelTerm, vec3(0.0), vec3(1.0));
		// clamp prevents overshoot on near 0 roughness (specNormTerm)
	
	
	
	// light color taking into account light color, light image and shadow. attenuation is handled separately
	vec3 lightColor = pLightColor;
	#ifdef GI_RAY
	lightColor += pLightColorAmbientGI;
	#endif
	
	#ifdef TEXTURE_COLOR
		// shadow maps are upside down compared to regular images. this is due to the images being
		// stored upside down to fit opengl. we need to reverse the flipping to get it right
		vec2 lightImageTC = shapos1.st;
		lightImageTC.y = 1.0 - lightImageTC.y;
		lightColor *= pow(texture(texColor, lightImageTC).rgb, vec3(pLightImageGamma));
		
	#elif defined TEXTURE_COLOR_CUBEMAP
		// the shadow matrix is world aligned but for the light image we need image aligned.
		// this is stored in a separate matrix present only if a light image is used
		vec3 lightImageTC = normalize(pLightImageOmniMatrix[vInLayer] * vec4(position, 1.0));
		lightColor *= pow(texture(texColorCubemap, lightImageTC).rgb, vec3(pLightImageGamma));
		
	#elif defined TEXTURE_COLOR_EQUIRECT
		// the shadow matrix is world aligned but for the light image we need image aligned.
		// this is stored in a separate matrix present only if a light image is used
		vec2 lightImageTC = equirectFromNormal(normalize(pLightImageOmniMatrix[vInLayer] * vec4(position, 1.0)));
		lightColor *= pow(texture(texColorEquirect, lightImageTC).rgb, vec3(pLightImageGamma));
	#endif
	
	#ifdef WITH_SUBSURFACE
		vec3 absorptionLightColor = lightColor;
	#endif
	
	#ifndef GI_RAY
		#ifdef PARTICLE_LIGHT
			lightColor *= vec3(dotval);
		#else
	 		//lightColor *= vec3(dotval);
			lightColor *= vec3(mix(pLightAmbientRatio, 1.0, dotval));
		#endif
	#endif
	#ifdef SHADOW_CASTING
		lightColor *= shadowColor;
	#endif
	
	#ifndef GI_RAY
		// apply ambient occlusion to the direct lighting. this is done by comparing the ambient occlusion angle
		// with the lighting angle. the ambient occlusion angle can be calculated using the relation ao = 1 - cos(angle).
		// this works because the ao value can be considered the ratio between a free sphere cap and the entire half
		// sphere. since a sphere cap is defined by an angle around the sphere direction the ao value can be directly
		// related to an angle with a simple calculation. this allows to add self-shadowing with next to no extra cost
		// 
		// with SSAO the result is tricky and more wrong than right in some cases. due to this only the texture-ao is
		// used for self-shadowing since this one is guaranteed to be well calculated and usually on a small scale
		// 
		lightColor *= vec3(clamp(((acos(1.0 - aoSolidity.r) - acos(dotval)) * pAOSelfShadow.y) + 1.0, pAOSelfShadow.x, 1.0));
		//lightColor *= vec3(smoothstep(pAOSelfShadow.x, 1.0, ((acos(1.0 - aoSolidity.r) - acos(dotval)) * pAOSelfShadow.y) + 1.0));
		//  problem with smoothstep undefined if pAOSelfShadow.x == 1.0 which is the case for disabling self-shadowing
		
		// ambient lighting. this uses the better ambient lighting trick. there the calculation would be like this:
		//   color += diffuse * ambient * mix(0.25, 1, clamp(d, 0, 1))
		// where d is:
		//   dnl = dot(normal, lightdir)
		//   d = squared(dnl * 0.5 + 0.5)
		// this can be rewritten a bit like this:
		//   d = dnl * 0.5 * dnl * 0.5 + 2 * dnl * 0.5 * 0.5 + 0.5 * 0.5
		//   d = dnl * dnl * 0.25 + dnl * 0.5 + 0.25
		//   d = dot((dnl * dnl, dnl, 1), (0.25, 0.5, 0.25))
		// this calculation is moved up since we need the dot product without clamping for the ambient lighting
		#ifdef AMBIENT_LIGHTING
			#ifdef SKY_LIGHT
				#ifdef ENABLE_OCCTRACING
					vec3 finalColorAmbient = pLightColorAmbient * vec3(aoSolidity.g);
					if(pOTEnabled){
						// normal is not a geometry normal but a modified normal. for tracing
						// we need though a geometry normal. this can be derived from depth
						//finalColorAmbient *= vec3(occtraceOcclusion(position, normal));
						finalColorAmbient *= vec3(occtraceOcclusion(position, normalFromDepth(tc, depth, position)));
					}
				#else
					vec3 finalColorAmbient = pLightColorAmbient * vec3(aoSolidity.g);
				#endif
			#else
	// 			vec3 finalColorAmbient = pLightColorAmbient * vec3(mix(0.25, 1.0, clamp(aldotval, 0.0, 1.0)) * aoSolidity.g);
				vec3 finalColorAmbient = pLightColorAmbient * vec3(aoSolidity.g);
			#endif
		#endif
	#endif
	
	// distance and spot attenuation
	#ifndef SKY_LIGHT
		#ifdef PARTICLE_LIGHT
			dist /= vParticleLightRange; // map all ranges into the 1m range so the same attenuation parameters can be used
		#endif
		float attenuation = max(pLightDampingCoefficient / (1.0 + pLightAttenuationCoefficient * dist * dist) + pLightDampingThreshold, 0.0);
		
		#ifdef SPOT_LIGHT
			// here comes the fun train!
			// 
			// for mathematicians 0 raised to the power of 0 is 1. unfortunately GLSL has an
			// entire different idea of what 0 raised to the power of 0 is and declares it
			// undefined. this results in a crappy situation which would never have existed
			// to begin with if GLSL would stay on the mathematician side.
			// 
			// the spot light calculation needs a term in the form of
			//   pow(dotSpot * spotFactor, spotExponent).
			// 
			// dotSpot is discarded if exactly 0 or less. dotSpot * spotFactor though can turn
			// 0 if spotFactor is small enough. in this situation is is possible to obtain
			// pow(0, 0) if the exponent is also 0.
			// 
			// for a mathematician this is not a problem as the result is one as with any
			// other value of dotSpot times spotFactor. Since GLSL though declares this
			// undefined we run into problems since nVidia for the first fucking time sticks
			// to GLSL specs to the word and trashes the rendered color texture whenever it
			// gets close to this situation.
			// 
			// there are now only two possible solutions.
			// 
			// one is to prevent the exponent from becoming 0. this is stupid since an
			// exponent of 0 is a typical solution to make a spot light with no light falloff
			// at all.
			// 
			// the other solution is to prevent the dotSpot times spotFactor to become 0. we
			// need a min statement there anyways to keep the value inside the 0 to 1 range.
			// this statement can be changed to also clamp against the 0 value problem. this
			// should work since a value close to 0 is not going to be noticed as not being 0.
			// values outside the spot cone are discard prior to this place anyways so this
			// is the best solution.
			//
			// NOTE this calculation is also no more working with spot cone being potentially
			//      squashed. the new version uses the same distance calculated for the spot
			//      cone rejection test above. the GLSL problem stays the same though
			#ifdef ANDROID
				// android GPU drivers tend to be buggy. if pow is used like on PC the shader
				// linker dead-locks and never returns. the culprit seems to be the pow()
				// function combined with certain uniform buffer variables. wrapping them in
				// float() calls breaks the dead-lock. if somebody can shed some light on this
				// strange driver bug he is welcome to tell me
				//attenuation *= pow(clamp(spotFactor * float(pLightSpotFactor), 0.001, 1.0), float(pLightSpotExponent));
				attenuation *= pow(clamp(spotConeRadius * pLightSpotFactor + pLightSpotBase,
					0.001, 1.0), float(pLightSpotExponent));
			#else
				//attenuation *= pow(clamp(spotFactor * pLightSpotFactor, 0.001, 1.0), pLightSpotExponent);
				attenuation *= pow(clamp(spotConeRadius * pLightSpotFactor + pLightSpotBase,
					0.001, 1.0), float(pLightSpotExponent));
			#endif
			
		#elif defined PROJECTOR_LIGHT
			// same as above but the calculation is a bit different in that is is applied
			// not on the radius but each direction individually
			vec2 spotConeAtten = pow(clamp(
				spotConeRadius * vec2(pLightSpotFactor) + vec2(pLightSpotBase),
				vec2(0.001), vec2(1.0)), vec2(pLightSpotExponent));
			attenuation *= spotConeAtten.x;
			attenuation *= spotConeAtten.y;
		#endif
		
		lightColor *= vec3(attenuation);
		#ifdef AMBIENT_LIGHTING
			finalColorAmbient *= vec3(attenuation);
		#endif
		#ifdef WITH_SUBSURFACE
			absorptionLightColor *= vec3(attenuation);
		#endif
	#endif
	
	// ambient occlusion
	//lightColor *= vec3(aoSolidity.g);
	//#ifdef AMBIENT_LIGHTING
	//	finalColorAmbient *= vec3(aoSolidity.g);
	//#endif
	#ifdef SKY_LIGHT
		#ifdef ENABLE_OCCTRACING
			if(pOTEnabled){
				lightColor = max(lightColor, finalColorAmbient);
			}
		#endif
	#endif
	
	// final light contribution
	finalColorSurface *= lightColor;
	
	vec3 finalColorSubSurface = lightColor;
	#ifdef AMBIENT_LIGHTING
		//#ifndef SKY_LIGHT
			// note: if enabled also for other light sources skip too if
			// ifdef ENABLE_OCCTRACING and pOTEnabled = true
			finalColorSubSurface += finalColorAmbient;
		//#endif
	#endif
	
	#ifndef GI_RAY
		outLuminance = vec4(vec3(dot(finalColorSubSurface + finalColorSurface, lumiFactors)), diffuse.a);
	#endif
	
	#ifdef WITH_SUBSURFACE
		outColor = vec4(finalColorSurface, diffuse.a);
		outSubSurface = vec4(finalColorSubSurface, diffuse.a);
		
		// add translucency SSS contribution. this is required since the SSS shader does not
		// know about translucency. to do this the thickness is calculated. this is an
		// approximation distance between the lit fragment and the matching shadow map fragment.
		// 
		// we need to use the SSS scatter calculation for this:
		//   attenuation = exp(scatterDistance * vec3(-5))
		// 
		// this value is for a scatterDistance up to 1m . to get the scatterDistance a
		// scatterScaling is required:
		//   scatterDistance = distance * scatterScale
		// 
		// scatterScale is an appropximation like this:
		//   scatterScale = vec3(1) / max(absorptionRadius, vec3(0.0001))
		// 
		// the absorptionDot is used to limit the effect to back facing fragments (as seen from
		// the light source). without this the front facing fragements would received additional
		// lighting which is wrong.
		
		// we have got a little problem with the use of 0 absorptionRadius which disables sss
		// altogether. due to the division below it has to be clamped to a value larger than 0
		// to avoid inf value which we can never get rid of anymore. doing so though makes 0
		// absorptionRadius to evaluate to full lighting being transmitted which is wrong. to
		// solve this problem the step() function is used which multiplies the sss contribution
		// by 0 if absorptionRadius is less than 1mm (for each component individually)
		
		vec3 scatDist = vec3(shadowThickness) / max(absorptionRadius, vec3(0.001));
		vec3 scatAdd = exp(scatDist * vec3(-5.0)) * vec3(absorptionDot) * absorptionLightColor;
		outSubSurface.rgb += scatAdd * step(vec3(0.001), absorptionRadius);
	#else
		outColor = vec4(finalColorSubSurface * diffuse.rgb + finalColorSurface, diffuse.a);
	#endif
}
