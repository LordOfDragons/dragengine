#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_gi.glsl"

UNIFORM_BIND(0) uniform mat4 pMatrixMVP;
UNIFORM_BIND(1) uniform int pGIDebugCascade;

#define pGIGridProbeCount pGIParams[pGIDebugCascade].probeCount
#define TEX_GI_PROBE_OFFSET_BINDING 0
#include "shared/defren/gi/probe_offset.glsl"

layout(location=0) in vec2 inPosition;

VARYING_BIND(0) out vec3 vColor;

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
	vec3 probePosition = pGIParams[pGIDebugCascade].probeSpacing * vec3( probeCoord );
	
	// first vertex is (-1,1), second is (1,1)
	if( inPosition.x < 0.0 ){
		vColor = vec3( 0.0, 0.0, 1.0 );
		
	}else{
		vColor = vec3( 1.0, 0.0, 0.0 );
		probePosition += gipoProbeOffset( giGridShiftToLocal( probeCoord ), pGIDebugCascade );
	}
	
	gl_Position = pMatrixMVP * vec4( probePosition, 1.0 );
}
