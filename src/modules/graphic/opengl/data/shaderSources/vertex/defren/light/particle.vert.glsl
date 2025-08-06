#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"

layout(binding=10) uniform mediump sampler2D texSamples;

layout(location=0) in vec4 inParticle0; // lifetime, position.x, position.y, position.z
layout(location=1) in vec4 inParticle1; // linearVelocity, angularVelocity, castSize, castEmissivity
layout(location=3) in vec4 inParticle3; // castRed, castGreen, castBlue, castTransparency
/*
// ParticleMode == ParticleModeBeam
layout(location=4) in float inParticle4; // beamLocation
*/

#include "shared/interface/light_vertex.glsl"

const vec2 curveOffset1 = vec2(0, 1.0 / 4.0);

// ParticleMode == ParticleModeBeam
const vec2 curveOffset2 = vec2(0, 2.0 / 4.0);
const vec2 curveOffset3 = vec2(0, 3.0 / 4.0);



void main(void){
	lightVertexShaderDefaultOutputs();
	
	// retrieves particle parameters from texSamples
	vec2 tcSamples = vec2(inParticle0.x, 0) * pSamplesParams.xz + pSamplesParams.yw;
	vec3 pattrs1 = vec3(texture(texSamples, tcSamples));
	vec3 pattrs2 = vec3(texture(texSamples, tcSamples + curveOffset1));
	vec3 pattrs3, pattrs4;
	
	if(ParticleMode == ParticleModeBeam){
		tcSamples.s = pBurstFactor * pSamplesParams.x + pSamplesParams.y;
		pattrs3 = vec3(texture(texSamples, tcSamples + curveOffset2));
		pattrs4 = vec3(texture(texSamples, tcSamples + curveOffset3));
	}
	
	vParticleLightColor = inParticle3.xyz * pattrs1.xyz; // red, green, blue
	vParticleLightColor *= vec3(inParticle1.w * pattrs2.y); // emissivity
	vParticleLightRange = inParticle1.z * pattrs2.z; // size
	
	if(ParticleMode == ParticleModeBeam){
		vParticleLightColor *= pattrs3.xyz; // red, green, blue
		vParticleLightColor *= vec3(pattrs4.y); // emissivity
		vParticleLightRange *= pattrs4.z; // size
	}
	
	vParticleLightRange *= 2.0; // 2 meters range per size meters
	
	// retrieves the position from the input particle
	gl_Position = vec4(inParticle0.yzw, 1);
	
	#ifdef SUPPORTS_VSDRAWPARAM
	if(VSRenderStereo){
		vLayer = gl_DrawID;
	}
	#endif
}
