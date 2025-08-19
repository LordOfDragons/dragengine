#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/trace_probe.glsl"
#include "shared/defren/gi/raycast/ray_cache.glsl"

layout(binding=0) uniform HIGHP sampler2DArray texCacheDistance;
layout(binding=1) uniform HIGHP sampler2DArray texCacheNormal;
layout(binding=2) uniform lowp sampler2DArray texCacheDiffuse;
layout(binding=3) uniform lowp sampler2DArray texCacheReflectivity;
layout(binding=4) uniform HIGHP sampler2DArray texCacheLight;

layout( binding=0, rgba16f ) uniform writeonly restrict HIGHP image2D texPosition;
layout( binding=1, rgba8_snorm ) uniform writeonly restrict HIGHP image2D texNormal;
layout( binding=2, rgba8 ) uniform writeonly restrict lowp image2D texDiffuse;
layout( binding=3, rgba8 ) uniform writeonly restrict lowp image2D texReflectivity;
layout( binding=4, rgba16f ) uniform writeonly restrict HIGHP image2D texLight;

layout( local_size_x=64 ) in;

void main( void ){
	ivec2 tc = ivec2( gl_GlobalInvocationID );
	
	int updateIndex = pGIProbesPerLine * tc.y + tc.x / pGIRaysPerProbe;
	if( updateIndex >= pGIProbeCount ){
		return;
	}
	
	int probeIndex = giTraceProbeProbeIndex( updateIndex );
	int rayIndex = tc.x % pGIRaysPerProbe;
	
	ivec3 tcCache = giRayCastCacheTCFromProbeIndex( probeIndex, rayIndex );
	
	// we have to reconstruct the position from the distance and the ray direction.
	// this is required since moving the GI field position renders the cached
	// positions invalid. the distance though stays valid
	float dist = texelFetch( texCacheDistance, tcCache, 0 ).r;
	vec3 position = vec3( pGIProbePosition[ updateIndex ] ) + pGIRayDirection[ rayIndex ] * dist;
	
	imageStore( texPosition, tc, vec4( position, dist ) );
	imageStore( texNormal, tc, texelFetch( texCacheNormal, tcCache, 0 ) );
	imageStore( texDiffuse, tc, texelFetch( texCacheDiffuse, tcCache, 0 ) );
	imageStore( texReflectivity, tc, texelFetch( texCacheReflectivity, tcCache, 0 ) );
	imageStore( texLight, tc, texelFetch( texCacheLight, tcCache, 0 ) );
}
