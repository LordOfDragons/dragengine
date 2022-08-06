#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif



// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"

#ifdef SHARED_SPB
	#include "v130/shared/defren/skin/shared_spb_index.glsl"
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif




// Samplers
/////////////

uniform mediump sampler2D texSamples;



// Inputs
///////////

in vec4 inParticle0; // lifetime, position.x, position.y, position.z
in vec4 inParticle1; // linearVelocity, angularVelocity, castSize, castEmissivity
in vec4 inParticle2; // linearDirection.x, linearDirection.y, linearDirection.z, rotation
in vec4 inParticle3; // castRed, castGreen, castBlue, castTransparency
/*
#ifdef PARTICLE_BEAM
in float inParticle4; // beamLocation
#endif
*/



// Outputs
////////////

out vec3 vParticle0; // size, emissivity, rotation
out vec4 vParticle1; // red, green, blue, transparency
#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif

#ifdef VS_RENDER_STEREO
	uniform int pDrawIDOffset;
	#define inLayer (gl_DrawID + pDrawIDOffset)
	out int vLayer;
#else
	const int inLayer = 0;
#endif



// Constants
//////////////

const float factorRotation = 6.2831853; // pi*2

const vec2 curveOffset1 = vec2( 0.0, 1.0 / 4.0 );

#ifdef PARTICLE_BEAM
const vec2 curveOffset2 = vec2( 0.0, 2.0 / 4.0 );
const vec2 curveOffset3 = vec2( 0.0, 3.0 / 4.0 );
#endif




// Main Function
//////////////////

void main( void ){
	#include "v130/shared/defren/skin/shared_spb_index2.glsl"
	
	vec2 tcSamples = vec2( inParticle0.x, 0.0 ) * pSamplesParams.xz + pSamplesParams.yw;
	vec3 pattrs1 = vec3( texture( texSamples, tcSamples ) );
	vec3 pattrs2 = vec3( texture( texSamples, tcSamples + curveOffset1 ) );
	
	#ifdef PARTICLE_BEAM
	tcSamples.s = pBurstFactor * pSamplesParams.x + pSamplesParams.y;
	vec3 pattrs3 = vec3( texture( texSamples, tcSamples + curveOffset2 ) );
	vec3 pattrs4 = vec3( texture( texSamples, tcSamples + curveOffset3 ) );
	#endif
	
	vParticle0.xy = inParticle1.zw * pattrs2.zy; // size, emissivity
	vParticle0.z = inParticle2.w * factorRotation; // rotation
	vParticle1.xyz = inParticle3.xyz * pattrs1.xyz; // red, green, blue
	vParticle1.w = inParticle3.w * pattrs2.x; // transparency
	
	#ifdef PARTICLE_BEAM
	vParticle0.xy *= pattrs4.zy; // size, emissivity
	vParticle1.xyz *= pattrs3.xyz; // red, green, blue
	vParticle1.w *= pattrs3.x; // transparency
	#endif
	
	gl_Position = vec4( pMatrixModel * vec4( inParticle0.yzw, 1 ), 1 );
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
