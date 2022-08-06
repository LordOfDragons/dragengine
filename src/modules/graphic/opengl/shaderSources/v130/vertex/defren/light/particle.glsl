#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_instance_parameters.glsl"

uniform mediump sampler2D texSamples;

in vec4 inParticle0; // lifetime, position.x, position.y, position.z
in vec4 inParticle1; // linearVelocity, angularVelocity, castSize, castEmissivity
in vec4 inParticle3; // castRed, castGreen, castBlue, castTransparency
/*
#ifdef PARTICLE_BEAM
in float inParticle4; // beamLocation
#endif
*/

out vec3 vGSParticleLightColor;
out float vGSParticleLightRange;

#ifdef VS_RENDER_STEREO
	out int vGSLayer;
#endif

const vec2 curveOffset1 = vec2( 0, 1.0 / 4.0 );

#ifdef PARTICLE_BEAM
const vec2 curveOffset2 = vec2( 0, 2.0 / 4.0 );
const vec2 curveOffset3 = vec2( 0, 3.0 / 4.0 );
#endif



void main( void ){
	// retrieves particle parameters from texSamples
	vec2 tcSamples = vec2( inParticle0.x, 0 ) * pSamplesParams.xz + pSamplesParams.yw;
	vec3 pattrs1 = vec3( texture( texSamples, tcSamples ) );
	vec3 pattrs2 = vec3( texture( texSamples, tcSamples + curveOffset1 ) );
	
	#ifdef PARTICLE_BEAM
	tcSamples.s = pBurstFactor * pSamplesParams.x + pSamplesParams.y;
	vec3 pattrs3 = vec3( texture( texSamples, tcSamples + curveOffset2 ) );
	vec3 pattrs4 = vec3( texture( texSamples, tcSamples + curveOffset3 ) );
	#endif
	
	vGSParticleLightColor = inParticle3.xyz * pattrs1.xyz; // red, green, blue
	vGSParticleLightColor *= vec3( inParticle1.w * pattrs2.y ); // emissivity
	vGSParticleLightRange = inParticle1.z * pattrs2.z; // size
	
	#ifdef PARTICLE_BEAM
	vGSParticleLightColor *= pattrs3.xyz; // red, green, blue
	vGSParticleLightColor *= vec3( pattrs4.y ); // emissivity
	vGSParticleLightRange *= pattrs4.z; // size
	#endif
	
	vGSParticleLightRange *= 2.0; // 2 meters range per size meters
	
	// retrieves the position from the input particle
	gl_Position = vec4( inParticle0.yzw, 1 );
	
	#ifdef VS_RENDER_STEREO
		vGSLayer = gl_DrawID;
	#endif
}
