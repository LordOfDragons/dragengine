#include "shared/preamble.glsl"

// request high precision if the graphic card supports this
precision HIGHP float;
precision HIGHP int;


// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"
#include "shared/defren/light/ubo_light_parameters.glsl"
#include "shared/equirectangular.glsl"


// Configuration
//////////////////

const bool HasShadow1 = TextureShadow1Solid || TextureShadow1Transparent;
const bool HasShadow2 = TextureShadow2Solid || TextureShadow2Transparent;
const bool HasShadows = HasShadow1 || HasShadow2;
const bool HasColoredShadows = TextureShadow1Transparent || TextureShadow2Transparent;

// shadow casting is currently not supported on particle lights
const bool CastShadows = HasShadows && LightMode != LightModeParticle;

const bool UseArrayForm = LightMode == LightModeSky && !GIRay;

#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
	#if LIGHT_MODE == 3 && ! defined GI_RAY
		#define USE_ARRAY_FORM
	#endif
#endif


// Samplers
/////////////

// see deoglLightShader::UpdateTextureTargets()

#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
	#if SHADOW1_MODE == 1
		#define RCS1C(e) e;
		#define RCS1A(e)
		#define RCS1R(e)
	#elif defined USE_ARRAY_FORM
		#define RCS1C(e)
		#define RCS1A(e) e;
		#define RCS1R(e)
	#else
		#define RCS1C(e)
		#define RCS1A(e)
		#define RCS1R(e) e;
	#endif
	
	#if SHADOW2_MODE == 1
		#define RCS2C(e) e;
		#define RCS2A(e)
		#define RCS2R(e)
	#elif defined USE_ARRAY_FORM
		#define RCS2C(e)
		#define RCS2A(e) e;
		#define RCS2R(e)
	#else
		#define RCS2C(e)
		#define RCS2A(e)
		#define RCS2R(e) e;
	#endif
	
	#if TEXTURE_LIGHT_COLOR == 1
		#define RCTLCR(e) e;
		#define RCTLCC(e)
		#define RCTLCE(e)
	#elif TEXTURE_LIGHT_COLOR == 2
		#define RCTLCR(e)
		#define RCTLCC(e) e;
		#define RCTLCE(e)
	#elif TEXTURE_LIGHT_COLOR == 3
		#define RCTLCR(e)
		#define RCTLCC(e)
		#define RCTLCE(e) e;
	#else
		#define RCTLCR(e)
		#define RCTLCC(e)
		#define RCTLCE(e)
	#endif
	
#else
	#define RCS1C(e) e;
	#define RCS1A(e) e;
	#define RCS1R(e) e;
	
	#define RCS2C(e) e;
	#define RCS2A(e) e;
	#define RCS2R(e) e;
	
	#define RCTLCR(e) e;
	#define RCTLCC(e) e;
	#define RCTLCE(e) e;
#endif

// GIRay
#if defined NVIDIA_SAMPLER_COUNT_WORKAROUND && ! defined GI_RAY
	#define texGIPosition texShadowNoise
	#define texGIDiffuse texShadowNoise
	#define texGINormal texShadowNoise
	#define texGIReflectivity texShadowNoise
#else
	layout(binding=0) uniform HIGHP sampler2D texGIPosition;
	layout(binding=1) uniform lowp sampler2D texGIDiffuse;
	layout(binding=2) uniform lowp sampler2D texGINormal;
	layout(binding=3) uniform lowp sampler2D texGIReflectivity; // reflectivity.rgb, roughness
#endif

// !GIRay
layout(binding=0) uniform HIGHP sampler2DArray texDepth;
layout(binding=1) uniform lowp sampler2DArray texDiffuse;
layout(binding=2) uniform lowp sampler2DArray texNormal;
layout(binding=3) uniform lowp sampler2DArray texReflectivity;
layout(binding=4) uniform lowp sampler2DArray texRoughness;
layout(binding=5) uniform lowp sampler2DArray texAOSolidity;

// WithSubsurface
layout(binding=6) uniform HIGHP sampler2DArray texSubSurface;

// WithSubsurface && TextureShadow1Solid
RCS1R(layout(binding=7) uniform HIGHP sampler2D texLightDepth1)
RCS1A(layout(binding=7) uniform HIGHP sampler2DArray texLightDepth1Array)
RCS1C(layout(binding=7) uniform HIGHP samplerCube texLightDepth1Cube)

// WithSubsurface && TextureShadow2Solid
RCS2R(layout(binding=8) uniform HIGHP sampler2D texLightDepth2)
RCS2A(layout(binding=8) uniform HIGHP sampler2DArray texLightDepth2Array)
RCS2C(layout(binding=8) uniform HIGHP samplerCube texLightDepth2Cube)

// TextureLightColor
RCTLCR(layout(binding=9) uniform lowp sampler2D texColor)
RCTLCC(layout(binding=9) uniform lowp samplerCube texColorCubemap)
RCTLCE(layout(binding=9) uniform lowp sampler2D texColorEquirect)

// TextureShadow1Solid
RCS1R(layout(binding=11) uniform HIGHP sampler2DShadow texShadow1SolidDepth)
RCS1A(layout(binding=11) uniform HIGHP sampler2DArrayShadow texShadow1SolidDepthArray)
RCS1C(layout(binding=11) uniform HIGHP samplerCubeShadow texShadow1SolidDepthCube)

// TextureShadow1Ambient
RCS1R(layout(binding=12) uniform HIGHP sampler2DShadow texShadow1Ambient)
RCS1A(layout(binding=12) uniform HIGHP sampler2DArrayShadow texShadow1AmbientArray)
RCS1C(layout(binding=12) uniform HIGHP samplerCubeShadow texShadow1AmbientCube)

// TextureShadow2Solid
RCS2R(layout(binding=13) uniform HIGHP sampler2DShadow texShadow2SolidDepth)
RCS2A(layout(binding=13) uniform HIGHP sampler2DArrayShadow texShadow2SolidDepthArray)
RCS2C(layout(binding=13) uniform HIGHP samplerCubeShadow texShadow2SolidDepthCube)

// TextureShadow2Ambient
RCS2R(layout(binding=14) uniform HIGHP sampler2DShadow texShadow2Ambient)
RCS2A(layout(binding=14) uniform HIGHP sampler2DArrayShadow texShadow2AmbientArray)
RCS2C(layout(binding=14) uniform HIGHP samplerCubeShadow texShadow2AmbientCube)

// TextureShadow1Transparent
RCS1R(layout(binding=15) uniform HIGHP sampler2DShadow texShadow1TransparentDepth)
RCS1A(layout(binding=15) uniform HIGHP sampler2DArrayShadow texShadow1TransparentDepthArray)
RCS1C(layout(binding=15) uniform HIGHP samplerCubeShadow texShadow1TransparentDepthCube)

// TextureShadow1Transparent
RCS1R(layout(binding=16) uniform lowp sampler2D texShadow1TransparentColor)
RCS1C(layout(binding=16) uniform lowp samplerCube texShadow1TransparentColorCube)

// TextureShadow2Transparent
RCS2R(layout(binding=17) uniform HIGHP sampler2DShadow texShadow2TransparentDepth)
RCS2A(layout(binding=17) uniform HIGHP sampler2DArrayShadow texShadow2TransparentDepthArray)
RCS2C(layout(binding=17) uniform HIGHP samplerCubeShadow texShadow2TransparentDepthCube)

// TextureShadow2Transparent
RCS2R(layout(binding=18) uniform lowp sampler2D texShadow2TransparentColor)
RCS2C(layout(binding=18) uniform lowp samplerCube texShadow2TransparentColorCube)

// NoiseTap
layout(binding=19) uniform lowp sampler2D texShadowNoise;


// Inputs
///////////

#include "shared/interface/light/fragment.glsl"

vec3 getLightPosition(){
	return LightMode == LightModeParticle ? vParticleLightPosition : pLightPosition[vLayer];
}
vec3 getLightColor(){
	return LightMode == LightModeParticle ? vParticleLightColor : pLightColor;
}
float getLightRange(){
	return LightMode == LightModeParticle ? vParticleLightRange : pLightRange;
}


// Outputs
////////////

layout(location=0) out vec4 outColor;

// !GIRay
//
// vec4 not float since blending is required. if float is used
// GPU randomly writes NaN values breaking rendering
layout(location=1) out vec4 outLuminance;

// WithSubsurface
layout(location=2) out vec4 outSubSurface;


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

const vec3 ambientLightFactor = vec3(0.25, 0.5, 0.25);

const vec3 lumiFactors = vec3(0.2125, 0.7154, 0.0721);
//const vec3 lumiFactors = vec3(0.3086, 0.6094, 0.0820); // nVidia

// calculate the solid shadow value. we use a little trick to avoid shadow artifacts as well as
// avoiding taping the shadow map for pixels it is not required. for this the observation is made
// that pixels facing away from the light are always in shadows. in this case there is no need
// for taping the shadow map since the result should be 0 in the correct case.
const bool OptimizeShadowBackface = false;

const bool EnableOccTracing = false;


// Includes using definitions above
/////////////////////////////////////

#include "shared/normal/texture.glsl"
#include "shared/defren/depth_to_position.glsl"
#include "shared/defren/depth_to_position_fragment.glsl"


// Shadow casting
///////////////////

#include "shared/defren/light/shadow/shadow.glsl"


const float defaultShadowThickness = 100.0; // just something large enough


float evalShadowMap(in ARG_SAMP_HIGHP sampler2DShadow texsm,
in ARG_SAMP_HIGHP sampler2DArrayShadow texsmarr, in vec3 params, in vec4 position){
	if(UseArrayForm){
		return evalShadowMapArray(texsmarr, params, position);
		
	}else{
		return evalShadowMap(texsm, params, vec3(position));
	}
}


float evalShadow1SolidDepth(in vec3 params, in vec4 position){
	#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
		#if SHADOW1_MODE == 1
			return 1.0;
		#elif defined USE_ARRAY_FORM
			return evalShadowMapArray(texShadow1SolidDepthArray, params, position);
		#else
			return evalShadowMap(texShadow1SolidDepth, params, vec3(position));
		#endif
	#else
		return evalShadowMap(texShadow1SolidDepth, texShadow1SolidDepthArray, params, position);
	#endif
}

float evalShadow1TransparentDepth(in vec3 params, in vec4 position){
	#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
		#if SHADOW1_MODE == 1
			return 1.0;
		#elif defined USE_ARRAY_FORM
			return evalShadowMapArray(texShadow1TransparentDepthArray, params, position);
		#else
			return evalShadowMap(texShadow1TransparentDepth, params, vec3(position));
		#endif
	#else
		return evalShadowMap(texShadow1TransparentDepth, texShadow1TransparentDepthArray, params, position);
	#endif
}

float evalShadow1Ambient(in vec3 params, in vec4 position){
	#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
		#if SHADOW1_MODE == 1
			return 1.0;
		#elif defined USE_ARRAY_FORM
			return evalShadowMapArray(texShadow1AmbientArray, params, position);
		#else
			return evalShadowMap(texShadow1Ambient, params, vec3(position));
		#endif
	#else
		return evalShadowMap(texShadow1Ambient, texShadow1AmbientArray, params, position);
	#endif
}


float evalShadow1SolidDepthCube(in vec3 params, in vec4 position){
	#if defined NVIDIA_SAMPLER_COUNT_WORKAROUND && SHADOW1_MODE != 1
		return 1.0f;
	#else
		return evalShadowCube(texShadow1SolidDepthCube, params, position);
	#endif
}

float evalShadow1TransparentDepthCube(in vec3 params, in vec4 position){
	#if defined NVIDIA_SAMPLER_COUNT_WORKAROUND && SHADOW1_MODE != 1
		return 1.0f;
	#else
		return evalShadowCube(texShadow1TransparentDepthCube, params, position);
	#endif
}

float evalShadow1AmbientCube(in vec3 params, in vec4 position){
	#if defined NVIDIA_SAMPLER_COUNT_WORKAROUND && SHADOW1_MODE != 1
		return 1.0f;
	#else
		return evalShadowCube(texShadow1AmbientCube, params, position);
	#endif
}


float evalShadow2SolidDepth(in vec3 params, in vec4 position){
	#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
		#if SHADOW2_MODE == 1
			return 1.0;
		#elif defined USE_ARRAY_FORM
			return evalShadowMapArray(texShadow2SolidDepthArray, params, position);
		#else
			return evalShadowMap(texShadow2SolidDepth, params, vec3(position));
		#endif
	#else
		return evalShadowMap(texShadow2SolidDepth, texShadow2SolidDepthArray, params, position);
	#endif
}

float evalShadow2TransparentDepth(in vec3 params, in vec4 position){
	#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
		#if SHADOW2_MODE == 1
			return 1.0;
		#elif defined USE_ARRAY_FORM
			return evalShadowMapArray(texShadow2TransparentDepthArray, params, position);
		#else
			return evalShadowMap(texShadow2TransparentDepth, params, vec3(position));
		#endif
	#else
		return evalShadowMap(texShadow2TransparentDepth, texShadow2TransparentDepthArray, params, position);
	#endif
}

float evalShadow2Ambient(in vec3 params, in vec4 position){
	#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
		#if SHADOW2_MODE == 1
			return 1.0;
		#elif defined USE_ARRAY_FORM
			return evalShadowMapArray(texShadow2AmbientArray, params, position);
		#else
			return evalShadowMap(texShadow2Ambient, params, vec3(position));
		#endif
	#else
		return evalShadowMap(texShadow2Ambient, texShadow2AmbientArray, params, position);
	#endif
}


float evalShadow2SolidDepthCube(in vec3 params, in vec4 position){
	#if defined NVIDIA_SAMPLER_COUNT_WORKAROUND && SHADOW2_MODE != 1
		return 1.0f;
	#else
		return evalShadowCube(texShadow2SolidDepthCube, params, position);
	#endif
}

float evalShadow2TransparentDepthCube(in vec3 params, in vec4 position){
	#if defined NVIDIA_SAMPLER_COUNT_WORKAROUND && SHADOW2_MODE != 1
		return 1.0f;
	#else
		return evalShadowCube(texShadow2TransparentDepthCube, params, position);
	#endif
}

float evalShadow2AmbientCube(in vec3 params, in vec4 position){
	#if defined NVIDIA_SAMPLER_COUNT_WORKAROUND && SHADOW2_MODE != 1
		return 1.0f;
	#else
		return evalShadowCube(texShadow2AmbientCube, params, position);
	#endif
}


// Discard fragment without disabling early-z
///////////////////////////////////////////////

void outputUnlit(){
	outColor = vec4(0);
	if(!GIRay){
		outLuminance = vec4(0);
	}
	if(WithSubsurface){
		outSubSurface = vec4(0);
	}
}


// Main Function
//////////////////

void main(void){
	ivec3 tcArray = ivec3(gl_FragCoord.xy, vLayer);
	ivec2 tcGI = ivec2(tcArray); // GIRay
	ivec3 tc = tcArray; // !GIRay
	
	// discard not inizalized fragments or fragements that are not supposed to be lit
	vec4 diffuse;
	if(GIRay){
		diffuse = texelFetch(texGIDiffuse, tcGI, 0);
		
	}else{
		diffuse = texelFetch(texDiffuse, tc, 0);
		if(diffuse.a == 0.0){
			outputUnlit();
			return;
		}
	}
	
	// determine position of fragment to light
	vec3 position;
	float depth;
	
	if(GIRay){
		vec4 positionDistance = texelFetch(texGIPosition, tcGI, 0);
		if(positionDistance.a > 9999.0){
			// ray hits nothing. for sky lights this adds the ambient light contribution.
			// for all other light sources do not light the ray
			if(LightMode == LightModeSky){
				vec3 lightColor = pLightColorAmbientGI;
				//outLuminance = vec4(vec3(dot(lightColor, lumiFactors)), diffuse.a);
				outColor = vec4(lightColor * diffuse.rgb, diffuse.a);
				
			}else{
				outputUnlit();
			}
			return;
		}
		position = vec3(pGIRayMatrix * vec4(positionDistance.rgb, 1.0));
		
	}else{
		depth = sampleDepth(texDepth, tc);
		if(FullScreenQuad){
			position = depthToPosition(depth, vScreenCoord, vLayer);
			
		}else{
			position = depthToPositionVolume(depth, vLightVolumePos, vLayer);
		}
	}
	
	// calculate light direction and distance
	vec3 lightDir;
	float dist;
	
	if(LightMode == LightModeSky){
		lightDir = pLightView[vLayer];
		
	}else{
		lightDir = getLightPosition() - position;
		dist = length(lightDir);
		
		// discard if pre-lit (length = 0) or outside the light range
		if(dist == 0.0 || dist > getLightRange()){
			outputUnlit();
			return;
		}
		lightDir /= dist;
		
		// discard fragments outside the spot cone
		// NOTE this does not work anymore since spot cone can be squashed now
		/*
		if(LightMode == LightModeSpot){
			float spotFactor = dot(lightView, -lightDir) - pLightSpotBase;
			if(spotFactor <= 0.0){
				discard;
			}
		}
		*/
	}
	
	// calculate shadow position if required
	float thicknessShadowScale;
	vec4 shapos1, shapos2;
	vec2 projectorConeRadius;
	float spotConeRadius;
	
	if(LightMode == LightModeSky){
		if(CastShadows){
			// WithSubsurface
			
			if(position.z < pLayerBorder.x){
				shapos1 = (pShadowMatrix1[vLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 0.0; // layer 0
				if(WithSubsurface){
					thicknessShadowScale = pShadowDepthTransform.z;
				}
				
			}else if(position.z < pLayerBorder.y){
				shapos1 = (pShadowMatrix2[vLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 1.0; // layer 1
				if(WithSubsurface){
					thicknessShadowScale = pShadowDepthTransform.w;
				}
				
			}else if(position.z < pLayerBorder.z){
				shapos1 = (pShadowMatrix3[vLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 2.0; // layer 2
				if(WithSubsurface){
					thicknessShadowScale = pShadowDepthTransform2.z;
				}
				
			}else{
				shapos1 = (pShadowMatrix4[vLayer] * vec4(position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 3.0; // layer 3
				if(WithSubsurface){
					thicknessShadowScale = pShadowDepthTransform2.w;
				}
			}
		}
		
	}else if(LightMode == LightModeParticle){
		shapos1 = vec4(position - vParticleLightPosition, 1);
		
	}else{
		shapos1 = pShadowMatrix1[vLayer] * vec4(position, 1);
		
		if(Shadow1Mode == ShadowModeCube){
			vec3 absshapos1 = abs(vec3(shapos1));
			float shaZ1 = max(max(absshapos1.x, absshapos1.y), max(absshapos1.z, pLightShadowClamp));
			shapos1.q = (shaZ1 * pShadowDepthTransform.x + pShadowDepthTransform.y) / shaZ1;
			
		}else{
			shapos1.pq = max(shapos1.pq, vec2(pLightShadowClamp));
			shapos1.stp /= shapos1.q;
		}
		
		if(LightMode == LightModeSpot){
			// discard fragments outside the spot cone. since spot cones can now be squashed
			// the shadow matrix texture coordinates have to be used to get a correct result.
			// this boils down to a circle test in the range 0..1 which is a simple check if
			// the point is more than 0.5 units away from the center. this is always correct
			// the same no matter how squashed the cone is
			spotConeRadius = length(shapos1.st - vec2(0.5));
			if(spotConeRadius > 0.5){
				outputUnlit();
				return;
			}
			
		}else if(LightMode == LightModeProjector){
			// discard fragments outide the projector frustum
			projectorConeRadius = abs(shapos1.st - vec2(0.5));
			if(any(greaterThan(projectorConeRadius, vec2(0.5)))){
				outputUnlit();
				return;
			}
		}
		
		if(CastShadows && TextureShadow2Solid){
			if(ShaMat2EqualsShaMat1){
				shapos2 = shapos1;
				
			}else{
				shapos2 = pShadowMatrix2[vLayer] * vec4(position, 1.0);
				
				if(Shadow2Mode == ShadowModeCube){
					//shapos2.w = length(shapos2.xyz);
					vec3 absshapos2 = abs(shapos2.xyz);
					float shaZ2 = max(max(absshapos2.x, absshapos2.y), max(absshapos2.z, pLightShadowClamp));
					shapos2.q = (shaZ2 * pShadowDepthTransform.x + pShadowDepthTransform.y) / shaZ2;
					
				}else{
					shapos2.pq = max(shapos2.pq, vec2(pLightShadowClamp));
					shapos2.stp /= shapos2.q;
				}
			}
		}
	}
	
	if(LightMode == LightModeSky && CastShadows){
		shapos2 = shapos1;
	}
	
	// fetch normal
	vec3 normal;
	if(GIRay){
		normal = normalLoadMaterial(texGINormal, tcGI);
		normal = normal * pGIRayMatrixNormal; // requires matrix transpose done by reversed order
		
	}else{
		normal = normalLoadMaterial(texNormal, tc);
	}
	
	if(dot(normal, normal) < 0.0001){
		normal = lightDir; // 0-normal means always point towards light source
		
	}else{
		normal = clamp(normal, vec3(-1.0), vec3(1.0)); // some shader writes broken normals (or missing clear?). temporary fix
		normal = normalize(normal);
	}
	
	// calculate the sss thickness from the shadow map if existing
	float largestAbsorptionRadius, shadowThickness;
	vec3 absorptionRadius;
	
	if(WithSubsurface){
		absorptionRadius = texelFetch(texSubSurface, tcArray, 0).rgb;
		largestAbsorptionRadius = max(max(absorptionRadius.x, absorptionRadius.y), absorptionRadius.z);
		shadowThickness = defaultShadowThickness;
		
		if(CastShadows){
			if(TextureShadow1Solid){
				if(Shadow1Mode == ShadowModeCube){
					RCS1C(shadowThickness = shadowCubeThickness(texLightDepth1Cube, shapos1, pShadowDepthTransform.zw))
					
				}else if(UseArrayForm){
					RCS1A(shadowThickness = shadowMapArrayThickness(texLightDepth1Array, thicknessShadowScale, shapos1))
					
				}else{
					RCS1R(shadowThickness = shadowMapThickness(texLightDepth1, vec3(shapos1), pShadowDepthTransform.zw))
				}
			}
			
			if(TextureShadow2Solid){
				if(Shadow2Mode == ShadowModeCube){
					RCS2C(shadowThickness = shadowCubeThickness(texLightDepth2Cube, shapos2, pShadowDepthTransform.zw))
					
				}else if(UseArrayForm){
					RCS2A(shadowThickness = shadowMapArrayThickness(texLightDepth2Array, thicknessShadowScale, shapos2))
					
				}else{
					RCS2R(shadowThickness = shadowMapThickness(texLightDepth2, vec3(shapos2), pShadowDepthTransform.zw))
				}
			}
		}
	}
	
	// calculate shadow color of required
	// NOTE the clamp() call is unfortunately mandatory. small imprecisions in GPU calculation
	//      can cause dot() to output a value which is slightly outside the range totally
	//      upsetting the upcoming calculations
	float dotval = clamp(dot(normal, lightDir), -1.0, 1.0);
	float absorptionDot = max(-dotval, 0.0); // WithSubsurface
	
	if(WithSubsurface){
		float v = abs(dotval);
		v *= v;
		v *= v; // pow(1-abs(dot), 4)
		absorptionDot *= v;
	}
	
	vec3 fullShadowColor = vec3(1.0), shadowColor = vec3(1.0);
	
	if(CastShadows){
		// calculate the solid shadow value. we use a little trick to avoid shadow artifacts as well as
		// avoiding taping the shadow map for pixels it is not required. for this the observation is made
		// that pixels facing away from the light are always in shadows. in this case there is no need
		// for taping the shadow map since the result should be 0 in the correct case.
		float shadow;
		
		if(OptimizeShadowBackface){
			if(dotval <= shadowThreshold){
				// discard is done later if shadow less than threshold
				shadow = 0.0;
				
			}else{
				shadow = clamp(dotval * shadowThresholdInv + 1.0, 0.0, 1.0);
			}
			
		}else{
			// force back facing fragments into shadow. not only does this avoid the need to
			// sample the shadow maps but it also avoids light leaking problems. this does
			// not affect ambient shadows
			shadow = dotval > 0.0 ? 1.0 : 0.0;
		}
		
		vec4 gishapos;
		
		if(shadow >= epsilon){
			if(TextureShadow1Solid){
				if(Shadow1Mode == ShadowModeCube){
					shadow = min(shadow, evalShadow1SolidDepthCube(pShadow1Solid, shapos1));
					
				}else if(GIRay && LightMode == LightModeSky){
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
						gishapos = pGIShadowMatrix * vec4(position, 1.0);
						shadow = min(shadow, evalShadow1SolidDepth(pGIShadowParams, gishapos));
						
						// prevent the test depth from reaching 0 or below. if this happens the test
						// result incorrectly considers the fragment not in shadows even if it is.
						// it is not enough to clamp to 0. it has to be larger than 0.
// 						shapos1.q = max(shapos1.q, 0.0001);
						
// 					}else{
// 						shadow = min(shadow, evalShadow1SolidDepth(texShadow1SolidDepth, pShadow1Solid, shapos1));
// 					}
					
				}else{
					shadow = min(shadow, evalShadow1SolidDepth(pShadow1Solid, shapos1));
				}
			}
			
			if(TextureShadow2Solid){
				if(Shadow2Mode == ShadowModeCube){
					shadow = min(shadow, evalShadow2SolidDepthCube(pShadow2Solid, shapos2));
					
				}else if(GIRay && LightMode == LightModeSky){
					shadow = min(shadow, evalShadow2SolidDepth(pGIShadowParams, gishapos));
					
				}else{
					shadow = min(shadow, evalShadow2SolidDepth(pShadow2Solid, shapos2));
				}
			}
			
			if(AmbientLighting){
				// temporary until sky light is improved. required since pLightAmbientRatio is
				// not defined for sky light which would result in strange errors if this not
				// initialized value is used
				fullShadowColor = vec3(shadow); // required for specular reflection
				
			}else if(TextureShadow1Ambient || TextureShadow2Ambient){
				// if shadow is 1 the fragment is in plain light and has to stay at 1
				// if shadow is 0 the fragment is in shadows. in this case it can received ambient light or not.
				// if ambient light applies it is applied using the ambient ratio parameter of the light source.
				// to avoid problems with light leaking through inside rooms having only a back facing wall the
				// ambient map is used like an additional shadow map before calculating the real result
				float ambientShadow = 1.0;
				
				if(TextureShadow1Ambient){
					if(Shadow1Mode == ShadowModeCube){
						ambientShadow = min(ambientShadow, evalShadow1AmbientCube(pShadow1Solid, shapos1));
						
					}else{
						ambientShadow = min(ambientShadow, evalShadow1Ambient(pShadow1Solid, shapos1));
					}
				}
				
				if(TextureShadow2Ambient){
					if(Shadow2Mode == ShadowModeCube){
						ambientShadow = min(ambientShadow, evalShadow2AmbientCube(pShadow2Solid, shapos2));
						
					}else{
						ambientShadow = min(ambientShadow, evalShadow2Ambient(pShadow2Solid, shapos2));
					}
				}
				
				fullShadowColor = vec3(shadow * ambientShadow); // required for specular reflection
				shadow = mix(pLightAmbientRatio, 1.0, shadow) * ambientShadow;
				/*
				if(Shadow1Mode == ShadowModeCube){
					shadow = mix(pLightAmbientRatio * evaluateShadowCube(texShadowAmbient, pShadow1Solid, shapos1), 1.0, shadow);
				}else{
					shadow = mix(pLightAmbientRatio * evaluateShadow2D(texShadowAmbient, pShadow1Solid, shapos1), 1.0, shadow);
				}
				*/
				
			}else{
				fullShadowColor = vec3(shadow); // required for specular reflection
				shadow = mix(pLightAmbientRatio, 1.0, shadow);
			}
		}
		
		// discard the fragment if it is fully in shadows and there is no ambient light
		if(!AmbientLighting){
			if(shadow < epsilon){
				bool discardFragment = true;
				if(WithSubsurface){
					discardFragment = shadowThickness > largestAbsorptionRadius;
				}
				if(discardFragment){
					outputUnlit();
					return;
				}
			}
		}
		
		if(GIRay){
			shadow = step(0.75, shadow);
		}
		
		shadowColor = vec3(shadow);
		
		// calculate the transparent shadow value
		if(HasColoredShadows){
			// only evaluate transparency if not in solid shadow
			if(shadow > epsilon){
				float transpShadow;
				vec4 transpColor;
				
				if(TextureShadow1Transparent){
					if(Shadow1Mode == ShadowModeCube){
						transpShadow = evalShadow1TransparentDepthCube(pShadow1Transparent, shapos1);
						RCS1C(transpColor = textureLod(texShadow1TransparentColorCube, shapos1.stp, 0.0))
						
					}else if(UseArrayForm){
						transpColor = vec4(0.0);
						
					}else{
						transpShadow = evalShadow1TransparentDepth(pShadow1Transparent, shapos1);
						RCS1R(transpColor = textureLod(texShadow1TransparentColor, shapos1.st, 0.0))
					}
					shadowColor *= mix(vec3(1.0), transpColor.rgb, vec3((1.0 - transpShadow) * transpColor.a));
				}
				
				if(TextureShadow2Transparent){
					if(Shadow2Mode == ShadowModeCube){
						transpShadow = evalShadow2TransparentDepthCube(pShadow2Transparent, shapos2);
						RCS2C(transpColor = textureLod(texShadow2TransparentColorCube, shapos2.stp, 0.0))
						
					}else if(UseArrayForm){
						transpColor = vec4(0.0);
						
					}else{
						transpShadow = evalShadow2TransparentDepth(pShadow2Transparent, shapos2);
						RCS2R(transpColor = textureLod(texShadow2TransparentColor, shapos2.st, 0.0))
					}
					shadowColor *= mix(vec3(1.0), transpColor.rgb, vec3((1.0 - transpShadow) * transpColor.a));
				}
			}
		}
	}
	
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
	vec3 reflectivity, roughness, aoSolidity;
	
	if(GIRay){
		vec4 reflRough = texelFetch(texGIReflectivity, tcGI, 0);
		reflectivity = reflRough.rgb;
		roughness = vec3(reflRough.a, 0.0, 0.0);
		aoSolidity = vec3(1.0);
		
	}else{
		reflectivity = vec3(texelFetch(texReflectivity, tc, 0));
		roughness = vec3(texelFetch(texRoughness, tc, 0));
		aoSolidity = vec3(texelFetch(texAOSolidity, tc, 0));
		
		// merge the texture-ao with the ssao. use the minimum of the two to avoid over-occluding
		// if both are used. the result is stored in aoSolidity.g . this way aoSolidity.r contains
		// the pure texture-ao and aoSolidity.gb the combined ao
		aoSolidity.g = min(aoSolidity.r, aoSolidity.g);
		diffuse.a *= aoSolidity.b;
	}
	
	// specular term
	//roughness.r = max(roughness.r, 0.1);
	float ap = 426.0 * (1.0 - roughness.r) / (90.0 * roughness.r * roughness.r + roughness.r + 0.001) + 1.0;
	vec3 halfDir = normalize(lightDir - normalize(position));
	float specNormTerm = (ap + 2.0) / 8.0;
	float specPowTerm = pow(clamp(dot(normal, halfDir), 0.0, 0.99), ap); // 0.99 prevents infinity overshoot on near 0-angle ray
	vec3 specFresnelTerm = mix(reflectivity, vec3(1.0), vec3(pow(clamp(1.0 - dot(lightDir, halfDir), 0.0, 1.0), 5.0)));
	
	//float aldotval = dot(vec3(dotval * dotval, dotval, 1.0), ambientLightFactor); //AmbientLighting
	
	dotval = max(dotval, 0.0);
	
	if(CastShadows){
		specFresnelTerm *= fullShadowColor; // no specular reflection in shadows
	}
	vec3 finalColorSurface = clamp(vec3(specNormTerm * specPowTerm) * specFresnelTerm, vec3(0.0), vec3(1.0));
		// clamp prevents overshoot on near 0 roughness (specNormTerm)
	
	
	// light color taking into account light color, light image and shadow. attenuation is handled separately
	vec3 lightColor = getLightColor();
	if(GIRay){
		lightColor += pLightColorAmbientGI;
	}
	
	if(TextureLightColor == TextureLightColor2D){
		// shadow maps are upside down compared to regular images. this is due to the images being
		// stored upside down to fit opengl. we need to reverse the flipping to get it right
		vec2 ltc = shapos1.st;
		ltc.y = 1.0 - ltc.y;
		RCTLCR(lightColor *= pow(texture(texColor, ltc).rgb, vec3(pLightImageGamma)))
		
	}else if(TextureLightColor == TextureLightColorCube){
		// the shadow matrix is world aligned but for the light image we need image aligned.
		// this is stored in a separate matrix present only if a light image is used
		vec3 ltc = normalize(pLightImageOmniMatrix[vLayer] * vec4(position, 1.0));
		RCTLCC(lightColor *= pow(texture(texColorCubemap, ltc).rgb, vec3(pLightImageGamma)))
		
	}else if(TextureLightColor == TextureLightColorEquirect){
		// the shadow matrix is world aligned but for the light image we need image aligned.
		// this is stored in a separate matrix present only if a light image is used
		vec2 ltc = equirectFromNormal(normalize(pLightImageOmniMatrix[vLayer] * vec4(position, 1.0)));
		RCTLCE(lightColor *= pow(texture(texColorEquirect, ltc).rgb, vec3(pLightImageGamma)))
	}
	
	// WithSubsurface
	vec3 absorptionLightColor = lightColor;
	
	if(!GIRay){
		if(LightMode == LightModeParticle){
			lightColor *= vec3(dotval);
			
		}else{
			//lightColor *= vec3(dotval);
			lightColor *= vec3(mix(pLightAmbientRatio, 1.0, dotval));
		}
	}
	if(CastShadows){
		lightColor *= shadowColor;
	}
	
	vec3 finalColorAmbient;
	
	if(!GIRay){
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
		if(AmbientLighting){
			if(LightMode == LightModeSky){
				/*
				if(EnableOccTracing){
					finalColorAmbient = pLightColorAmbient * vec3(aoSolidity.g);
					if(pOTEnabled){
						// normal is not a geometry normal but a modified normal. for tracing
						// we need though a geometry normal. this can be derived from depth
						//finalColorAmbient *= vec3(occtraceOcclusion(position, normal));
						finalColorAmbient *= vec3(occtraceOcclusion(position, normalFromDepth(tc, depth, position)));
					}
				}else{
				*/
					finalColorAmbient = pLightColorAmbient * vec3(aoSolidity.g);
				//}
				
			}else{
	// 			vec3 finalColorAmbient = pLightColorAmbient * vec3(mix(0.25, 1.0, clamp(aldotval, 0.0, 1.0)) * aoSolidity.g);
				finalColorAmbient = pLightColorAmbient * vec3(aoSolidity.g);
			}
		}
	}
	
	// distance and spot attenuation
	if(LightMode != LightModeSky){
		if(LightMode == LightModeParticle){
			dist /= vParticleLightRange; // map all ranges into the 1m range so the same attenuation parameters can be used
		}
		float attenuation = max(pLightDampingCoefficient / (1.0 + pLightAttenuationCoefficient * dist * dist) + pLightDampingThreshold, 0.0);
		
		if(LightMode == LightModeSpot){
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
			
		}else if(LightMode == LightModeProjector){
			// same as above but the calculation is a bit different in that is is applied
			// not on the radius but each direction individually
			vec2 spotConeAtten = pow(clamp(
				projectorConeRadius * vec2(pLightSpotFactor) + vec2(pLightSpotBase),
				vec2(0.001), vec2(1.0)), vec2(pLightSpotExponent));
			attenuation *= spotConeAtten.x;
			attenuation *= spotConeAtten.y;
		}
		
		lightColor *= vec3(attenuation);
		if(AmbientLighting){
			finalColorAmbient *= vec3(attenuation);
		}
		if(WithSubsurface){
			absorptionLightColor *= vec3(attenuation);
		}
	}
	
	// ambient occlusion
	//lightColor *= vec3(aoSolidity.g);
	//if(AmbientLighting){
	//	finalColorAmbient *= vec3(aoSolidity.g);
	//}
	/*
	if(LightMode == LightModeSky && EnableOccTracing){
		//if(pOTEnabled){
			lightColor = max(lightColor, finalColorAmbient);
		//}
	}
	*/
	
	// final light contribution
	finalColorSurface *= lightColor;
	
	vec3 finalColorSubSurface = lightColor;
	if(AmbientLighting){
		//if(LightMode != LightModeSky){
			// note: if enabled also for other light sources skip too if
			// ifdef EnableOccTracing and pOTEnabled = true
			finalColorSubSurface += finalColorAmbient;
		//}
	}
	
	if(!GIRay){
		outLuminance = vec4(vec3(dot(finalColorSubSurface + finalColorSurface, lumiFactors)), diffuse.a);
	}
	
	if(WithSubsurface){
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
		
	}else{
		outColor = vec4(finalColorSubSurface * diffuse.rgb + finalColorSurface, diffuse.a);
	}
}
