#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/trace_probe.glsl"
#include "shared/image_buffer.glsl"

#include "shared/defren/gi/raycast/ray_cache.glsl"


// WithRayCache
layout(binding=0, IMG_R16F_FMT) uniform readonly HIGHP IMG_R16F_2DARR texCacheDistance;

// !WithRayCache
layout(binding=0, rgba16f) uniform readonly HIGHP image2D texPosition;


struct sProbeExtends {
	vec3 minExtend;
	vec3 maxExtend;
};

UBOLAYOUT_BIND(0) writeonly restrict buffer ProbeExtends {
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
	
	ivec2 rayOffset;
	if(WithRayCache){
		rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( index ) );
	}else{
		rayOffset = ivec2( ( index % pGIProbesPerLine ) * pGIRaysPerProbe, index / pGIProbesPerLine );
	}
	
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
			vec3 position;
			
			if(WithRayCache){
				ivec3 rayTC = ivec3( rayOffset + ivec2( rayIndex, 0 ), pGICascade );
				position = probePosition + pGIRayDirection[ rayIndex ]
					* IMG_R16F_LOAD(imageLoad(texCacheDistance, rayTC));
			}else{
				position = vec3( imageLoad( texPosition, rayOffset + ivec2( rayIndex, 0 ) ) );
			}
			
			vRayData[ gl_LocalInvocationIndex ].minExtend = position;
			vRayData[ gl_LocalInvocationIndex ].maxExtend = position;
		}
		barrier();
		
		// per invocation processing. combine all results
		for( i=0; i<combineParams64Count; i++ ){
			combineRays( combineParams64Mul[ i ] * uvec3( gl_LocalInvocationIndex ) + combineParams64Add[ i ] );
			barrier();
		}
		combineRays( combineParams64Last );
		barrier();
		
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
				vec3 position = probePosition + pGIRayDirection[i]
					* IMG_R16F_LOAD(imageLoad(texCacheDistance, rayTC));
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
