precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"
#include "v130/shared/defren/gi/raycast/ray_cache.glsl"

uniform mediump sampler2DArray texPosition;
uniform mediump sampler2DArray texNormal;
uniform lowp sampler2DArray texDiffuse;
uniform lowp sampler2DArray texReflectivity;
uniform mediump sampler2DArray texLight;

out vec4 outPosition;
out vec3 outNormal;
out vec3 outDiffuse;
out vec4 outReflectivity;
out vec3 outLight;

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, pGICascade );
	
	int rowProbeIndex = tc.x / pGIRaysPerProbe;
	int instanceID = pGIProbesPerLine * tc.y + rowProbeIndex;
	if( instanceID >= pGIProbeCount ){
		discard;
	}
	
	int firstRayOffset = pGIRaysPerProbe * rowProbeIndex;
	int rayIndex = tc.x - firstRayOffset;
	
	// calculate probe index from instance ID. this is the real probe index
	int probeIndex = giTraceProbeProbeIndex( instanceID );
	
	tc.xy = giRayCastCacheTCFromProbeIndex( probeIndex, rayIndex );
	
	// we have to reconstruct the position from the distance and the ray direction.
	// this is required since moving the GI field position renders the cached
	// positions invalid. the distance though stays valid
	outPosition.w = texelFetch( texPosition, tc, 0 ).r;
	outPosition.xyz = pGIProbePosition[ instanceID ].xyz + pGIRayDirection[ rayIndex ] * outPosition.w;
	
	outNormal = texelFetch( texNormal, tc, 0 ).rgb;
	outDiffuse = texelFetch( texDiffuse, tc, 0 ).rgb;
	outReflectivity = texelFetch( texReflectivity, tc, 0 );
	outLight = texelFetch( texLight, tc, 0 ).rgb;
}
