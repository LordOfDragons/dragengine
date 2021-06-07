precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

in vec3 inPosition;

out ivec2 vGSRayOffset;

void main( void ){
	int probeIndex = pGIProbeIndex[ gl_InstanceID >> 2 ][ gl_InstanceID & 3 ]; // 4 IDs per array entry
	
	ivec2 offset = ivec2( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ), probeIndex / pGIProbesPerLine );
	
	vGSRayOffset = ivec2( ( gl_InstanceID % pGIProbesPerLine ) * pGIRaysPerProbe, gl_InstanceID / pGIProbesPerLine ) - offset;
	
	vec2 realSize = vec2( pGIRaysPerProbe, 1 ) * pGIRayMapScale;
	vec2 realOffset = vec2( offset ) * pGIRayMapScale * vec2( 2.0 ) + realSize + vec2( -1.0 );
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
