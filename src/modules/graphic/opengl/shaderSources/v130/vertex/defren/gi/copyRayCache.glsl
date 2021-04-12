precision highp float;
precision highp int;

#ifdef FROM_TRACE_TO_CACHE
	#include "v130/shared/ubo_defines.glsl"
	#include "v130/shared/defren/gi/ubo_gi.glsl"
#endif

in vec3 inPosition;

#ifdef FROM_TRACE_TO_CACHE
	flat out ivec2 vRayOffset;
#endif

void main( void ){
	#ifdef FROM_TRACE_TO_CACHE
		int probeIndex = pGIProbeIndex[ gl_InstanceID >> 2 ][ gl_InstanceID & 3 ]; // 4 IDs per array entry
		
		ivec2 offset = ivec2( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ), probeIndex / pGIProbesPerLine );
		
		vRayOffset = ivec2( ( gl_InstanceID % pGIProbesPerLine ) * pGIRaysPerProbe, gl_InstanceID / pGIProbesPerLine ) - offset;
		
		vec2 realSize = vec2( pGIRaysPerProbe, 1 ) * pGIRayMapScale;
		vec2 realOffset = vec2( offset ) * pGIRayMapScale * vec2( 2.0 ) + realSize + vec2( -1.0 );
		
		gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
		
	#else
		gl_Position = vec4( vec3( inPosition.xy, 0.0 ), 1.0 );
	#endif
}
