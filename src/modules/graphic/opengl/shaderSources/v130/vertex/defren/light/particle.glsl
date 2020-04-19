#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#define USE_INSTANCED_PARTICLES 1

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_instance_parameters.glsl"

#ifdef USE_INSTANCED_PARTICLES
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
	
	const vec2 curveOffset1 = vec2( 0.0, 1.0 / 4.0 );
	
	#ifdef PARTICLE_BEAM
	const vec2 curveOffset2 = vec2( 0.0, 2.0 / 4.0 );
	const vec2 curveOffset3 = vec2( 0.0, 3.0 / 4.0 );
	#endif
	
#else
	uniform vec3 pParticlePosition; // red, green, blue, range
	uniform vec4 pParticleLight; // red, green, blue, range
	#define pParticleLightColor vec3( pParticleLight )
	#define pParticleLightRange pParticleLight.w
	
	in vec3 inPosition;
	in vec2 inTexCoord;
	
	out vec3 vLightVolumePos;
	out vec3 vParticleLightPosition;
	out vec3 vParticleLightColor;
	out float vParticleLightRange;
	
	const vec2 tcOffset = vec2( -0.5 );
#endif



void main( void ){
	#ifdef USE_INSTANCED_PARTICLES
		// retrieves particle parameters from texSamples
		vec2 tcSamples = vec2( inParticle0.x, 0.0 ) * pSamplesParams.xz + pSamplesParams.yw;
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
		gl_Position = vec4( pMatrixMV * vec4( inParticle0.yzw, 1.0 ), 1.0 );
		
	#else
		vParticleLightPosition = pMatrixMV * vec4( pParticlePosition, 1.0 );
		vParticleLightColor = pParticleLightColor;
		vParticleLightRange = pParticleLightRange;
		
		vLightVolumePos = vParticleLightPosition;
		vLightVolumePos.xy += ( inTexCoord + tcOffset ) * vec2( pParticleLightRange * 2.0 );
		
		gl_Position = pMatrixMVP * vec4( vLightVolumePos, 1.0 );
	#endif
}



#if 0
uniform mat4x3 pMatrixMV;
uniform mat4 pMatrixProj;
uniform vec3 pPosition;
uniform float pSize;

in vec3 inPosition;
in vec2 inTexCoord;

out vec3 vLightVolumePos;
out vec3 vLightPos;
out vec3 vLightAttenuation;

const vec2 tcOffset = vec2( -0.5 );

void main( void ){
	vec3 position = pMatrixMV * vec4( pPosition /*inPosition*/, 1.0 );
	
	vLightPos = position;
	
	position.xy += ( inTexCoord + tcOffset ) * vec2( pSize * 2.0 );
	
	gl_Position = pMatrixProj * vec4( position, 1.0 );
	vLightVolumePos = position;
	
	float sizeSquared = pSize * pSize;
	//vLightAttenuation.x = ( 1.0 / ( halfIntDist * halfIntDist ) - 2.0 ) / sizeSquared;
	//vLightAttenuation.x = 98.0 / sizeSquared; // halfIntDist = 0.1
	vLightAttenuation.x = 23.0 / sizeSquared; // halfIntDist = 0.2
	float rangeNormInt = 1.0 / ( 1.0 + vLightAttenuation.x * sizeSquared );
	vLightAttenuation.y = 1.0 / ( 1.0 - rangeNormInt );
	vLightAttenuation.z = -rangeNormInt * vLightAttenuation.y;
}

#endif
