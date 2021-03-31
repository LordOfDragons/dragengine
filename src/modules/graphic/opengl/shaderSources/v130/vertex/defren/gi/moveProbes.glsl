precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

in vec3 inPosition;

out int vInstanceID;
out ivec3 vProbeCoord;


ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

void main( void ){
	vInstanceID = gl_InstanceID;
	
	int probeIndex = giTraceProbeProbeIndex( gl_InstanceID );
	vProbeCoord = probeIndexToGridCoord( probeIndex );
	
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * vProbeCoord.y + vProbeCoord.x, vProbeCoord.z );
	
	vec2 scale = vec2( 1.0 ) / vec2( pGIGridProbeCount.x * pGIGridProbeCount.y, pGIGridProbeCount.z );
	#define realSize scale
	vec2 realOffset = vec2( texCoord ) * scale * vec2( 2.0 ) + realSize + vec2( -1.0 );
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
