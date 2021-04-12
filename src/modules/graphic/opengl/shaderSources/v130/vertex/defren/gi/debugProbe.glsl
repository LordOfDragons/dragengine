#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/probe_offset.glsl"

uniform mat4 pMatrixMVP;
uniform ivec3 pGIGridCoordShift; // grid coordinate shift (wrapping around)

in vec3 inPosition;

flat out ivec3 vProbeCoord;
flat out vec3 vProbePosition;
out vec3 vDirection;

const float sphereScale = 0.05;

ivec3 giIndexToCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

ivec3 giGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pGIGridCoordShift ) % pGIGridProbeCount;
}

void main( void ){
	ivec3 probeCoord = giIndexToCoord( gl_InstanceID );
	vProbeCoord = giGridShiftToLocal( probeCoord );
	vProbePosition = pGIGridProbeSpacing * vec3( probeCoord ) + gipoProbeOffset( vProbeCoord );
	vDirection = inPosition;
	gl_Position = pMatrixMVP * vec4( inPosition * sphereScale + vProbePosition, 1.0 );
}
