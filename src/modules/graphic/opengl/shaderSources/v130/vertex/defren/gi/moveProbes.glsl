precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

in vec3 inPosition;

out ivec2 vRayOffset;
out vec3 vProbePosition;

ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

void main( void ){
	vRayOffset.x = ( gl_InstanceID % pGIProbesPerLine ) * pGIRaysPerProbe;
	vRayOffset.y = gl_InstanceID / pGIProbesPerLine;
	vProbePosition = pGIProbePosition[ gl_InstanceID ].xyz;
	
	int probeIndex = pGIProbeIndex[ gl_InstanceID >> 2 ][ gl_InstanceID & 3 ]; // 4 IDs per array entry
	ivec3 probeCoord = probeIndexToGridCoord( probeIndex );
	
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z );
	
	vec2 scale = vec2( 1.0 ) / vec2( pGIGridProbeCount.x * pGIGridProbeCount.y, pGIGridProbeCount.z );
	#define realSize scale
	vec2 realOffset = vec2( texCoord ) * scale * vec2( 2.0 ) + realSize + vec2( -1.0 );
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
