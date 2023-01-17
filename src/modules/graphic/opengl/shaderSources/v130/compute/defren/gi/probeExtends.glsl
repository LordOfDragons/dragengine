precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

#ifdef WITH_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache.glsl"
#endif


#ifdef WITH_RAY_CACHE
	layout(binding=0, rgba16f) uniform readonly image2DArray texPosition;
#else
	layout(binding=0, rgba16f) uniform readonly image2D texPosition;
#endif


struct sProbeExtends {
	vec3 minExtend;
	vec3 maxExtend;
};

UBOLAYOUT_BIND(0) writeonly buffer ProbeExtends {
	sProbeExtends pProbeExtends[];
};


layout( local_size_x=64 ) in;


void main( void ){
	int index = int( gl_GlobalInvocationID.x );
	
	if( index >= pRayCacheProbeCount ){
		return;
	}
	
	vec3 minExtend = pGIDetectionBox;
	vec3 maxExtend = -pGIDetectionBox;
	
	#ifdef WITH_RAY_CACHE
		ivec2 rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( index ) );
	#else
		ivec2 rayOffset = ivec2( ( index % pGIProbesPerLine ) * pGIRaysPerProbe, index / pGIProbesPerLine );
	#endif
	
	vec3 probePosition = vec3( pGIProbePosition[ index ] );
	int i;
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		#ifdef WITH_RAY_CACHE
			ivec3 rayTC = ivec3( rayOffset + ivec2( i, 0 ), pGICascade );
			vec3 position = probePosition + pGIRayDirection[ i ] * imageLoad( texPosition, rayTC ).r;
		#else
			vec3 position = vec3( imageLoad( texPosition, rayOffset + ivec2( i, 0 ) ) );
		#endif
		
		minExtend = min( minExtend, position );
		maxExtend = max( maxExtend, position );
	}
	
	pProbeExtends[ index ].minExtend = max( minExtend, -pGIDetectionBox );
	pProbeExtends[ index ].maxExtend = min( maxExtend, pGIDetectionBox );
}
