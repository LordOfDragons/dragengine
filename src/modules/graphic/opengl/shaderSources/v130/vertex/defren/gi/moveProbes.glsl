precision highp float;
precision highp int;

#include "v130/shared/uniform_const.glsl"
#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

UBOLAYOUT uniform GINewOffsetState{
	vec4 pGINewOffsetState[ GI_MAX_COUNT_PROBE_POSITIONS ]; // position(xyz), flags(w)
};


out vec4 vGSOffset; // offset(xyz), flags(w)


ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

void main( void ){
	int probeIndex = giTraceProbeProbeIndex( gl_InstanceID );
	ivec3 probeCoord = probeIndexToGridCoord( probeIndex );
	
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z );
	
	UFCONST vec2 scale = vec2( 1.0 ) / vec2( pGIGridProbeCount.x * pGIGridProbeCount.y, pGIGridProbeCount.z );
	const vec2 offset = vec2( -1.0 );
	vec2 realOffset = ( vec2( texCoord ) * vec2( 2.0 ) + vec2( 1.0 ) ) * scale + offset;
	
	gl_Position = vec4( realOffset, 0.0, 1.0 );
	
	vGSOffset = pGINewOffsetState[ gl_InstanceID ];
}
