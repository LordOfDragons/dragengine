#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;


// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_index.glsl"
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif

#include "shared/defren/skin/shared_spb_texture_redirect.glsl"
#include "shared/defren/skin/ubo_special_parameters.glsl"




// Samplers
/////////////

uniform mediump sampler2D texSamples;



// Inputs
///////////

layout(location=0) in vec4 inParticle0; // lifetime, position.x, position.y, position.z
layout(location=1) in vec4 inParticle1; // linearVelocity, angularVelocity, castSize, castEmissivity
layout(location=2) in vec4 inParticle2; // linearDirection.x, linearDirection.y, linearDirection.z, rotation
layout(location=3) in vec4 inParticle3; // castRed, castGreen, castBlue, castTransparency
/*
#ifdef PARTICLE_BEAM
layout(location=4) in float inParticle4; // beamLocation
#endif
*/



// Outputs
////////////

out vec3 vParticle0; // size, emissivity, rotation
out vec4 vParticle1; // red, green, blue, transparency
flat out int vParticleSheetCount;
flat out int vGSSPBIndex;
flat out int vGSSPBFlags;
flat out int vLayer;

#ifdef VS_RENDER_STEREO
	uniform int pDrawIDOffset;
	#define inLayer (gl_DrawID + pDrawIDOffset)
#else
	const int inLayer = 0;
#endif



// Constants
//////////////

const float factorRotation = 6.2831853; // pi*2

const vec2 curveOffset1 = vec2(0.0, 1.0 / 4.0);
const vec2 curveOffset2 = vec2(0.0, 2.0 / 4.0);
const vec2 curveOffset3 = vec2(0.0, 3.0 / 4.0);



// Main Function
//////////////////

void main(void){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	vec2 tcSamples = vec2(inParticle0.x, 0.0) * pSamplesParams.xz + pSamplesParams.yw;
	vec3 pattrs1 = vec3(texture(texSamples, tcSamples));
	vec3 pattrs2 = vec3(texture(texSamples, tcSamples + curveOffset1));
	
	vParticle0.xy = inParticle1.zw * pattrs2.zy; // size, emissivity
	vParticle0.z = inParticle2.w * factorRotation; // rotation
	vParticle1.xyz = inParticle3.xyz * pattrs1.xyz; // red, green, blue
	vParticle1.w = inParticle3.w * pattrs2.x; // transparency
	
	// beam only
	tcSamples.s = pBurstFactor * pSamplesParams.x + pSamplesParams.y;
	vec3 pattrs3 = vec3(texture(texSamples, tcSamples + curveOffset2));
	vec3 pattrs4 = vec3(texture(texSamples, tcSamples + curveOffset3));
	
	vParticle0.xy *= pattrs4.zy; // size, emissivity
	vParticle1.xyz *= pattrs3.xyz; // red, green, blue
	vParticle1.w *= pattrs3.x; // transparency
	// end beam only
	
	vParticleSheetCount = pTexParticleSheetCount;
	
	gl_Position = vec4(pMatrixModel * vec4(inParticle0.yzw, 1.0), 1.0);
	
	vGSSPBIndex = spbIndex;
	vGSSPBFlags = spbFlags;
	vLayer = inLayer;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
	#endif
}
