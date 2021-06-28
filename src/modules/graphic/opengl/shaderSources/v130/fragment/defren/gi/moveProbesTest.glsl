// own version of moveProbesTest. tries to move probes using information of all rays
// instead of only the closest and farthest ones

precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/probe_flags.glsl"

#ifdef WITH_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache.glsl"
	#include "v130/shared/defren/gi/trace_probe.glsl"
#endif


uniform sampler2D texPosition;
uniform sampler2D texNormal;


flat in int vInstanceID;
flat in ivec3 vProbeCoord;

out vec4 outOffset; // offset(xyz), flags(w)


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
	uint flags = uint( pGIProbePosition[ vInstanceID ].w );
	
	float backFaceMinDistToSurface = pGIMoveMinDistToSurface + 0.05;
	float closestFrontfaceDistance = 10000.0;
	float backfaceCount = 0.0;
	float countOffsets = 0.0;
	int i;
	
	outOffset = vec4( 0.0, 0.0, 0.0, flags );
	
	// if we apply the offset of all back faces we end up in troubles. for example if
	// two back faces face each other the offset towards the other side of both faces
	// cancels out causing the probe to not move at all. what we do here is finding the
	// closest back face and applying the offset only if it is the closest.
	//float closestBackDistance = backFaceMinDistToSurface;
	float closestBackDistance = 10000.0;
	vec3 closestBackOffset = vec3( 0.0 );
	
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
			closestFrontfaceDistance = min( closestFrontfaceDistance, rayDistance );
			
			if( rayDistance >= pGIMoveMinDistToSurface ){
				continue; // far enough to have no influence
				
			}else if( rayDistance < 0.001 ){
				// at surface. move along normal
				outOffset.xyz += hitNormal * pGIMoveMinDistToSurface;
				
			}else{
				// move back on ray until pGIMoveMinDistToSurface distance
				// offset = rayDirection - normalize(rayDirection) * pGIMoveMinDistToSurface
				// offset = normalize(rayDirection) * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection / rayLength * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection * ((rayLength - pGIMoveMinDistToSurface) / rayLength)
				outOffset.xyz += rayDirection * ( 1.0 - pGIMoveMinDistToSurface / rayDistance );
			}
			
		}else{
			backfaceCount += 1.0;
			
			if( rayDistance < closestBackDistance ){
				closestBackOffset = rayDirection + hitNormal * pGIMoveMinDistToSurface;
				closestBackDistance = rayDistance;
			}
		}
		
		countOffsets += 1.0;
	}
	
	bool assumeInGeometry = backfaceCount / float( pGIRaysPerProbe ) > 0.25;
	
	if( closestBackDistance < 9999.0 && assumeInGeometry ){
		outOffset.xyz = closestBackOffset;
		
	}else if( countOffsets > 0.0 ){
		outOffset.xyz /= countOffsets;
		
		// averaging the offsets has the tendency to shorten the offset if multiple
		// rays contribute. reduce this effect by enlaring the offset with larger
		// counts. a base scaling is always applied to reduce the chance of moving
		// the probe again in the future
		//outOffset.xyz *= 1.0 + 0.05 * ( countOffsets - 1 );
		outOffset.xyz *= 1.0 + 0.05 * countOffsets;
	}
	
	// position used for ray tracing contains the previous update probe offset.
	// the previous offset has to be added to the new offset for it to be correct.
	// vProbeCoord is local. we need it though shifted
	vec3 gridPosition = pGIGridProbeSpacing * vec3( giGridLocalToShift( vProbeCoord ) ) + pGIGridOrigin;
	outOffset.xyz += probePosition - gridPosition;
	
	// clamp offset
	outOffset.xyz = clamp( outOffset.xyz, -pGIMoveMaxOffset, pGIMoveMaxOffset );
// 		outOffset.xyz = vec3(0); // DEBUG
	
	// update flags
	/*
	uint flags = uint( pGIProbePosition[ vInstanceID ].w );
	
	// in the original source code "spacing * 2 * 1.45" is used. but I think only
	// "spacing * (1 + 0.45 * 2)" aka "spacing * 1.9" is required. the thinking is this:
	// if a surface is farther away than "spacing" then it falls into the next cell and this
	// probe has no effect on it anymore no matter if by direct lighting nor ray lighting.
	// offset can be at most 0.45 . so if both probes are have maximum offset but in opposite
	// direction then this yields "1 + 0.45 + 0.45" times the spacing which is "spacing * 1.9"
	const vec3 geometryBounds = pGIGridProbeSpacing * 1.9;
	
	if( all( lessThanEqual( vec3( closestFrontface.w ), geometryBounds ) ) && ! assumeInGeometry ) {
		flags &= ~gipfDisabled;
		
	}else{
		flags |= gipfDisabled;
	}
	
	outOffset.w = float( flags );
	*/
}
