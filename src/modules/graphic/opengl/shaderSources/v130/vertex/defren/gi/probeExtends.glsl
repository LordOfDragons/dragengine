precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

#ifdef WITH_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache.glsl"
#endif

#ifdef WITH_RAY_CACHE
	uniform sampler2DArray texPosition;
#else
	uniform sampler2D texPosition;
#endif

out vec3 fbMinExtend;
out vec3 fbMaxExtend;

void main( void ){
	gl_Position = vec4( 2, 2, 2, 1 ); // keep broken compilers happy
	fbMinExtend = pGIFieldSize;
	fbMaxExtend = -pGIFieldSize;
	
	#ifdef WITH_RAY_CACHE
		ivec2 rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( gl_InstanceID ) );
	#else
		ivec2 rayOffset = ivec2( ( gl_InstanceID % pGIProbesPerLine ) * pGIRaysPerProbe, gl_InstanceID / pGIProbesPerLine );
	#endif
	
	vec3 probePosition = pGIProbePosition[ gl_InstanceID ].xyz;
	int i;
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		#ifdef WITH_RAY_CACHE
			ivec3 rayTC = ivec3( rayOffset + ivec2( i, 0 ), pGICascade );
		#else
			ivec2 rayTC = rayOffset + ivec2( i, 0 );
		#endif
		
		#ifdef WITH_RAY_CACHE
			float rayDistance = texelFetch( texPosition, rayTC, 0 ).r;
		#else
			vec4 rayPosition = texelFetch( texPosition, rayTC, 0 ); // position, distance
			float rayDistance = rayPosition.w;
		#endif
		
		#ifdef WITH_RAY_CACHE
			vec3 position = probePosition + pGIRayDirection[ i ] * rayDistance;
		#else
			vec3 position = rayPosition.xyz;
		#endif
		
		fbMinExtend = min( fbMinExtend, position );
		fbMaxExtend = max( fbMaxExtend, position );
	}
	
	fbMinExtend = max( fbMinExtend, -pGIFieldSize );
	fbMaxExtend = min( fbMaxExtend, pGIFieldSize );
}
