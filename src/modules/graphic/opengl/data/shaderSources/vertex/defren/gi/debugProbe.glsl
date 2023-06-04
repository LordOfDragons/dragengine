#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_gi.glsl"

uniform mat4x3 pMatrixMV;
uniform mat4 pMatrixP;
uniform int pGIDebugCascade;

#define pGIGridProbeCount pGIParams[pGIDebugCascade].probeCount
#include "shared/defren/gi/probe_offset.glsl"

in vec2 inPosition;

flat out ivec3 vProbeCoord;
flat out vec3 vProbePosition;
out vec2 vTexCoord;
flat out float vRimThickness;

ivec3 giIndexToCoord( in int index ){
	int stride = pGIParams[pGIDebugCascade].probeCount.x * pGIParams[pGIDebugCascade].probeCount.z;
	return ivec3( index % pGIParams[pGIDebugCascade].probeCount.x,
		index / stride, ( index % stride ) / pGIParams[pGIDebugCascade].probeCount.x );
}

ivec3 giGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pGIParams[pGIDebugCascade].gridCoordShift ) % pGIParams[pGIDebugCascade].probeCount;
}

void main( void ){
	ivec3 probeCoord = giIndexToCoord( gl_InstanceID );
	vProbeCoord = giGridShiftToLocal( probeCoord );
	vProbePosition = pGIParams[pGIDebugCascade].probeSpacing * vec3( probeCoord )
		+ gipoProbeOffset( vProbeCoord, pGIDebugCascade );
	vTexCoord = inPosition;
	
	vec3 center = pMatrixMV * vec4( vProbePosition, 1.0 );
	
	vRimThickness = mix( 0.98, 0.5, clamp( center.z / 15.0, 0.0, 1.0 ) );
	
	const float sphereScale = 0.05;
	gl_Position = pMatrixP * vec4( center + vec3( inPosition * sphereScale, 0.0 ), 1.0 );
}
