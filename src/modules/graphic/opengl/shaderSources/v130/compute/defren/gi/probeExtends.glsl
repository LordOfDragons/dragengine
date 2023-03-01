precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

#ifdef WITH_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache.glsl"
#endif


#ifdef WITH_RAY_CACHE
	layout(binding=0, r16f) uniform readonly image2DArray texCacheDistance;
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


shared sProbeExtends vRayData[ 64 ];


void combineRays( in uvec3 params ){
	if( gl_LocalInvocationIndex < params.x ){
		vRayData[ params.y ].minExtend = min( vRayData[ params.y ].minExtend, vRayData[ params.z ].minExtend );
		vRayData[ params.y ].maxExtend = max( vRayData[ params.y ].maxExtend, vRayData[ params.z ].maxExtend );
	}
}


void main( void ){
	int index = int( gl_WorkGroupID.x );
	
	#ifdef WITH_RAY_CACHE
		ivec2 rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( index ) );
	#else
		ivec2 rayOffset = ivec2( ( index % pGIProbesPerLine ) * pGIRaysPerProbe, index / pGIProbesPerLine );
	#endif
	
	vec3 probePosition = vec3( pGIProbePosition[ index ] );
	int i;
	
	UFCONST int rayGroupCount = ( pGIRaysPerProbe - 1 ) / 64 + 1;
	vec3 minExtend = pGIDetectionBox;
	vec3 maxExtend = -pGIDetectionBox;
	int rg;
	
	for( rg=0; rg<rayGroupCount; rg++ ){
		int rayFirst = 64 * rg;
		
		// cooperative processing
		int rayIndex = rayFirst + int( gl_LocalInvocationIndex );
		
		if( rayIndex < pGIRaysPerProbe ){
			#ifdef WITH_RAY_CACHE
				ivec3 rayTC = ivec3( rayOffset + ivec2( rayIndex, 0 ), pGICascade );
				vec3 position = probePosition + pGIRayDirection[ rayIndex ] * imageLoad( texCacheDistance, rayTC ).r;
			#else
				vec3 position = vec3( imageLoad( texPosition, rayOffset + ivec2( rayIndex, 0 ) ) );
			#endif
			
			vRayData[ gl_LocalInvocationIndex ].minExtend = position;
			vRayData[ gl_LocalInvocationIndex ].maxExtend = position;
		}
		barrier(); memoryBarrier();
		
		// per invocation processing. combine all results
		for( i=0; i<combineParams64Count; i++ ){
			combineRays( combineParams64Mul[ i ] * uvec3( gl_LocalInvocationIndex ) + combineParams64Add[ i ] );
			barrier(); memoryBarrier();
		}
		combineRays( combineParams64Last );
		barrier(); memoryBarrier();
		
		// apply. this does not require invocation masking since we use only invoc[0] in the end
		minExtend = min( minExtend, vRayData[ 0 ].minExtend );
		maxExtend = max( maxExtend, vRayData[ 0 ].maxExtend );
	}
	
	// write probe parameters. this has to be done by exactly one invocation
	if( gl_LocalInvocationIndex == uint( 0 ) ){
		/* ground truth
		minExtend = pGIDetectionBox;
		maxExtend = -pGIDetectionBox;
		for(i=0; i<pGIRaysPerProbe; i++){
			#ifdef WITH_RAY_CACHE
				ivec3 rayTC = ivec3(rayOffset + ivec2(i,0), pGICascade);
				vec3 position = probePosition + pGIRayDirection[i] * imageLoad(texCacheDistance, rayTC).r;
			#else
				vec3 position = vec3(imageLoad(texPosition, rayOffset + ivec2(i,0)));
			#endif
			minExtend = min(minExtend, position);
			maxExtend = max(maxExtend, position);
		}
		*/
		
		pProbeExtends[ index ].minExtend = max( minExtend, -pGIDetectionBox );
		pProbeExtends[ index ].maxExtend = min( maxExtend, pGIDetectionBox );
	}
}
