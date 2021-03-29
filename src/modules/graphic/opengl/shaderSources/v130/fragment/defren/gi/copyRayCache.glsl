precision highp float;
precision highp int;

#ifndef FROM_TRACE_TO_CACHE
	#include "v130/shared/ubo_defines.glsl"
	#include "v130/shared/defren/gi/ubo_gi.glsl"
#endif

uniform mediump sampler2D texPosition;
uniform mediump sampler2D texNormal;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texReflectivity;
uniform mediump sampler2D texLight;

#ifdef FROM_TRACE_TO_CACHE
	flat in ivec2 vRayOffset;
#endif

#ifdef FROM_TRACE_TO_CACHE
	out float outPosition;
#else
	out vec4 outPosition;
#endif
out vec3 outNormal;
out vec3 outDiffuse;
out vec4 outReflectivity;
out vec3 outLight;

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	#ifdef FROM_TRACE_TO_CACHE
		tc += vRayOffset;
		
	#else
		int rowProbeIndex = tc.x / pGIRaysPerProbe;
		int instanceID = pGIProbesPerLine * tc.y + rowProbeIndex;
		if( instanceID >= pGIProbeCount ){
			discard;
		}
		
		int firstRayOffset = pGIRaysPerProbe * rowProbeIndex;
		int rayIndex = tc.x - firstRayOffset;
		
		// calculate probe index from instance ID. this is the real probe index
		int probeIndex = pGIProbeIndex[ instanceID >> 2 ][ instanceID & 3 ]; // 4 IDs per array entry
		
		// calculatee texture coordinate to sample. probes are packed one after the other
		// starting at the bottom plane going all the way up
		tc = ivec2( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ), probeIndex / pGIProbesPerLine );
		
		// offset texture coordinate by ray to query
		tc.x += rayIndex;
	#endif
	
	#ifdef FROM_TRACE_TO_CACHE
		outPosition = texelFetch( texPosition, tc, 0 ).a;
	#else
		// we have to reconstruct the position from the distance and the ray direction.
		// this is required since moving the GI field position renders the cached
		// positions invalid. the distance though stays valid
		outPosition.w = texelFetch( texPosition, tc, 0 ).r;
		outPosition.xyz = pGIProbePosition[ instanceID ].xyz + pGIRayDirection[ rayIndex ] * outPosition.w;
	#endif
	
	outNormal = texelFetch( texNormal, tc, 0 ).rgb;
	outDiffuse = texelFetch( texDiffuse, tc, 0 ).rgb;
	outReflectivity = texelFetch( texReflectivity, tc, 0 );
	outLight = texelFetch( texLight, tc, 0 ).rgb;
}
