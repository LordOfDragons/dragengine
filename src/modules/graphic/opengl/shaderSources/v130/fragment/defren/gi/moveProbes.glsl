precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

#ifdef WITH_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache.glsl"
	#include "v130/shared/defren/gi/trace_probe.glsl"
#endif


uniform sampler2D texPosition;
uniform sampler2D texNormal;


flat in int vInstanceID;
flat in ivec3 vProbeCoord;

out vec3 outOffset;


ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIGridCoordUnshift ) % pGIGridProbeCount;
}

void main( void ){
	#ifdef WITH_RAY_CACHE
		ivec2 rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( vInstanceID ) );
	#else
		ivec2 rayOffset = ivec2( ( vInstanceID % pGIProbesPerLine ) * pGIRaysPerProbe, vInstanceID / pGIProbesPerLine );
	#endif
	
	vec3 probePosition = pGIProbePosition[ vInstanceID ].xyz;
	
	float backFaceMinDistToSurface = pGIMoveMinDistToSurface + 0.05;
	int frontFaceCount = 0;
	int backfaceCount = 0;
	int countOffsets = 0;
	int i;
	
	outOffset = vec3( 0.0 );
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		ivec2 rayTC = rayOffset + ivec2( i, 0 );
		
		#ifdef WITH_RAY_CACHE
			float rayDistance = texelFetch( texPosition, rayTC, 0 ).r;
		#else
			vec4 rayPosition = texelFetch( texPosition, rayTC, 0 ); // position, distance
			#define rayDistance rayPosition.w
		#endif
		
		if( rayDistance >= 9999.0 ){
			continue; // ray misses. do not move
		}
		
		#ifdef WITH_RAY_CACHE
			vec3 rayDirection = pGIRayDirection[ i ] * rayDistance;
		#else
			vec3 rayDirection = rayPosition.xyz - probePosition;
		#endif
		
		vec3 hitNormal = texelFetch( texNormal, rayTC, 0 ).xyz;
		float distToSurface = dot( hitNormal, rayDirection );
		
		if( distToSurface < 0.0 ){
			frontFaceCount++;
			
			if( rayDistance >= pGIMoveMinDistToSurface ){
				continue; // far enough to have no influence
				
			}else if( rayDistance < 0.001 ){
				// at surface. move along normal
				outOffset += hitNormal * pGIMoveMinDistToSurface;
				
			}else{
				// move back on ray until pGIMoveMinDistToSurface distance
				// offset = rayDirection - normalize(rayDirection) * pGIMoveMinDistToSurface
				// offset = normalize(rayDirection) * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection / rayLength * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection * ((rayLength - pGIMoveMinDistToSurface) / rayLength)
				outOffset += rayDirection * ( 1.0 - pGIMoveMinDistToSurface / rayDistance );
			}
			
		}else{
			backfaceCount++;
			
			if( rayDistance >= backFaceMinDistToSurface ){
				continue; // too far away to reach other side. do not move
				
			}else{
				outOffset += rayDirection + hitNormal * pGIMoveMinDistToSurface;
			}
			/*
			vec3 offset = rayDirection + hitNormal * pGIMoveMinDistToSurface;
			offset = clamp( offset, -pGIMoveMaxOffset, pGIMoveMaxOffset );
			if( dot( hitNormal, offset ) - distToSurface < 0.05 ){
				continue; // can not reach the other side. do not move
			}
			*/
		}
		
		countOffsets++;
	}
	
	// TODO using back/front face count to disable probes
	
	if( countOffsets > 0 ){
		outOffset /= float( countOffsets );
		
		// averaging the offsets has the tendency to shorten the offset if multiple
		// rays contribute. reduce this effect by enlaring the offset with larger
		// counts. a base scaling is always applied to reduce the chance of moving
		// the probe again in the future
		//outOffset *= 1.0 + 0.05 * ( countOffsets - 1 );
		outOffset *= 1.0 + 0.05 * countOffsets;
	}
	
	// position used for ray tracing contains the previous update probe offset.
	// the previous offset has to be added to the new offset for it to be correct.
	// vProbeCoord is local. we need it though shifted
	vec3 gridPosition = pGIGridProbeSpacing * vec3( giGridLocalToShift( vProbeCoord ) ) + pGIGridOrigin;
	outOffset += probePosition - gridPosition;
	
	// clamp offset
	outOffset = clamp( outOffset, -pGIMoveMaxOffset, pGIMoveMaxOffset );
// 		outOffset = vec3(0);
}
