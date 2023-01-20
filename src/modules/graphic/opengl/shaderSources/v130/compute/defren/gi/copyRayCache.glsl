precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"
#include "v130/shared/defren/gi/raycast/ray_cache.glsl"

uniform mediump sampler2D texPosition;
uniform mediump sampler2D texNormal;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texReflectivity;
uniform mediump sampler2D texLight;

layout( binding=0, r16f ) uniform writeonly image2DArray texCacheDistance;
layout( binding=1, rgba8_snorm ) uniform writeonly image2DArray texCacheNormal;
layout( binding=2, rgba8 ) uniform writeonly image2DArray texCacheDiffuse;
layout( binding=3, rgba8 ) uniform writeonly image2DArray texCacheReflectivity;
layout( binding=4, rgba16f ) uniform writeonly image2DArray texCacheLight;

layout( local_size_x=64 ) in;

void main( void ){
	ivec2 tc = ivec2( gl_GlobalInvocationID );
	
	int updateIndex = pGIProbesPerLine * tc.y + tc.x / pGIRaysPerProbe;
	if( updateIndex >= pRayCacheProbeCount ){
		return;
	}
	
	int probeIndex = giTraceProbeProbeIndex( updateIndex );
	int rayIndex = tc.x % pGIRaysPerProbe;
	
	ivec3 tcCache = giRayCastCacheTCFromProbeIndex( probeIndex, rayIndex );
	
	imageStore( texCacheDistance, tcCache, vec4( texelFetch( texPosition, tc, 0 ).a ) );
	imageStore( texCacheNormal, tcCache, texelFetch( texNormal, tc, 0 ) );
	imageStore( texCacheDiffuse, tcCache, texelFetch( texDiffuse, tc, 0 ) );
	imageStore( texCacheReflectivity, tcCache, texelFetch( texReflectivity, tc, 0 ) );
	imageStore( texCacheLight, tcCache, texelFetch( texLight, tc, 0 ) );
}
