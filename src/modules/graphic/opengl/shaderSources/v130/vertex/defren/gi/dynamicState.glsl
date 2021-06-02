precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

flat out int vInstanceID;
flat out ivec3 vProbeCoord;

ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

void main( void ){
	// vertex shader part
	vInstanceID = gl_InstanceID;
	int probeIndex = giTraceProbeProbeIndex( gl_InstanceID );
	vProbeCoord = probeIndexToGridCoord( probeIndex );
	
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * vProbeCoord.y + vProbeCoord.x, vProbeCoord.z );
	
	const vec2 scale = vec2( 1.0 ) / vec2( pGIGridProbeCount.x * pGIGridProbeCount.y, pGIGridProbeCount.z );
	const vec2 offset = vec2( -1.0 );
	vec2 realOffset = ( vec2( texCoord ) * vec2( 2.0 ) + vec2( 1.0 ) ) * scale + offset;
	
	gl_Position = vec4( realOffset, 0.0, 1.0 );
}
