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
uniform usampler2D texState; // dynamic state from previous dynamicState.glsl run


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
	
	float closestFrontfaceDistance = 10000.0;
	float backfaceCount = 0.0;
	float countOffsets = 0.0;
	int i;
	
	outOffset = vec4( 0.0 );
	
	// position used for ray tracing contains the previous update probe offset.
	// the previous offset has to be added to the new offset for it to be correct.
	// vProbeCoord is local. we need it though shifted
	vec3 gridPosition = pGIGridProbeSpacing * vec3( giGridLocalToShift( vProbeCoord ) ) + pGIGridOrigin;
	vec3 prevOffset = probePosition - gridPosition;
	
	// max offset test scaling factor
	vec3 moveMaxOffsetFactor = vec3( 1.0 ) / pGIMoveMaxOffset;
	
	// if we apply the offset of all back faces we end up in troubles. for example if
	// two back faces face each other the offset towards the other side of both faces
	// cancels out causing the probe to not move at all. what we do here is finding the
	// closest back face and applying the offset only if it is the closest.
	float closestBackDistance = 10000.0;
	vec3 closestBackOffset = vec3( 10000.0 ); // very large to force disable if no good hit found
	
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
				vec3 direction = rayDirection + hitNormal * pGIMoveMinDistToSurface;
				
				if( length( ( prevOffset + direction ) * moveMaxOffsetFactor ) < 1.0 ){
					// consider backface ray only if not leaving allowed area
					closestBackOffset = direction;
					closestBackDistance = rayDistance;
				}
			}
		}
		
		countOffsets += 1.0;
	}
	
	bool assumeInGeometry = backfaceCount / float( pGIRaysPerProbe ) > 0.25;
	
	if( assumeInGeometry ){
		// closest backface found and more than 25% of hits are backface hits. as a first step
		// we want to move to the other side of the face to end up at a minimum distance from
		// the surface. if this is not possible do not move and disable the probe
		outOffset.xyz = closestBackOffset;
		
	}else if( countOffsets > 0.5 ){
		// front faces are hit
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
	outOffset.xyz += prevOffset;
	
	uint flags = uint( pGIProbePosition[ vInstanceID ].w );
	flags &= ~( gipfDisabled | gipfDynamicDisable );
	
	// in the original source code "spacing * 2 * 1.45" is used. but I think only
	// "spacing * (1 + 0.45 * 2)" aka "spacing * 1.9" is required. the thinking is this:
	// if a surface is farther away than "spacing" then it falls into the next cell and this
	// probe has no effect on it anymore no matter if by direct lighting nor ray lighting.
	// offset can be at most 0.45 . so if both probes are have maximum offset but in opposite
	// direction then this yields "1 + 0.45 + 0.45" times the spacing which is "spacing * 1.9"
	const vec3 nearGeometryRange = pGIGridProbeSpacing + pGIMoveMaxOffset * 2.0;
	
	if( all( lessThanEqual( vec3( closestFrontfaceDistance ), nearGeometryRange ) ) && ! assumeInGeometry ) {
		flags |= gipfNearGeometry;
	}
	
	// if frontfaces are hit clamp offset to maximum radius around grid position.
	// if backfaces are hit do not move if outside maximum radius
	vec3 gridOffset = outOffset.xyz * moveMaxOffsetFactor;
	float gridOffsetLen = length( gridOffset );
	
	if( gridOffsetLen > 1.0 ){
		if( assumeInGeometry ){
			// offset due to backface hit would move outside allowed range. do not move and
			// disable the probe. this also prevents future processing of the probe
			outOffset.xyz = prevOffset;
			flags |= gipfDisabled;
			
		}else{
			// offset due to frontface hit would move outside allowed range.
			// clamp the offset to the allowed range.
			if( gridOffsetLen > 0.001 ){
				outOffset.xyz *= min( gridOffsetLen, 1.0 ) / gridOffsetLen;
			}
		}
	}
	
	// merge dynamic state
	if( ( flags & ( gipfNearGeometry | gipfDisabled ) ) == uint( 0 ) ){
		flags |= texelFetch( texState, ivec2( gl_FragCoord.xy ), 0 ).r;
	}
	
	// write out flags
	outOffset.w = float( flags );
	
	// debug
// 	outOffset.xyz = vec4( 0.0, 0.0, 0.0, pGIProbePosition[ vInstanceID ].w );
}
