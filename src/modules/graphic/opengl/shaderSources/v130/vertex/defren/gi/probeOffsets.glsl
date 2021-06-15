precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/probe_flags.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

#ifdef WITH_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache.glsl"
#endif


#ifdef WITH_RAY_CACHE
	uniform sampler2DArray texPosition;
	uniform sampler2DArray texNormal;
#else
	uniform sampler2D texPosition;
	uniform sampler2D texNormal;
#endif

uniform usampler2D texState; // dynamic state from previous dynamicState.glsl run


out vec4 fbOffset; // offset(xyz), flags(w)
out vec4 fbOffsetCopy; // offset(xyz), flags(w)


ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIGridCoordUnshift ) % pGIGridProbeCount;
}

void main( void ){
	gl_Position = vec4( 2, 2, 0, 1 ); // keep broken compilers happy
	
	ivec3 probeCoord = probeIndexToGridCoord( giTraceProbeProbeIndex( gl_InstanceID ) );
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z );
	
	#ifdef WITH_RAY_CACHE
		ivec2 rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( gl_InstanceID ) );
	#else
		ivec2 rayOffset = ivec2( ( gl_InstanceID % pGIProbesPerLine ) * pGIRaysPerProbe, gl_InstanceID / pGIProbesPerLine );
	#endif
	
	vec3 probePosition = pGIProbePosition[ gl_InstanceID ].xyz;
	
	float closestFrontfaceDistance = 10000.0;
	float frontfaceCount = 0.0;
	float backfaceCount = 0.0;
	float countOffsets = 0.0;
	int i;
	
	fbOffset = vec4( 0.0 );
	
	// position used for ray tracing contains the previous update probe offset.
	// the previous offset has to be added to the new offset for it to be correct.
	// probeCoord is local. we need it though shifted
	vec3 gridPosition = pGIGridProbeSpacing * vec3( giGridLocalToShift( probeCoord ) ) + pGIGridOrigin;
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
		#ifdef WITH_RAY_CACHE
			ivec3 rayTC = ivec3( rayOffset + ivec2( i, 0 ), pGICascade );
		#else
			ivec2 rayTC = rayOffset + ivec2( i, 0 );
		#endif
		
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
			frontfaceCount += 1.0;
			closestFrontfaceDistance = min( closestFrontfaceDistance, rayDistance );
			
			if( rayDistance >= pGIMoveMinDistToSurface ){
				continue; // far enough to have no influence
				
			}else if( rayDistance < 0.001 ){
				// at surface. move along normal
				fbOffset.xyz += hitNormal * pGIMoveMinDistToSurface;
				
			}else{
				// move back on ray until pGIMoveMinDistToSurface distance
				// offset = rayDirection - normalize(rayDirection) * pGIMoveMinDistToSurface
				// offset = normalize(rayDirection) * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection / rayLength * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection * ((rayLength - pGIMoveMinDistToSurface) / rayLength)
				fbOffset.xyz += rayDirection * ( 1.0 - pGIMoveMinDistToSurface / rayDistance );
			}
			
		}else{
			backfaceCount += 1.0;
			
			if( rayDistance < closestBackDistance ){
				// the first version moves quite a distance to the other side of the face.
				// especially with highest cascades this can result in probes not moving
				// to the front side because they end up outside the allowed range. by using
				// a small distance in front of the face allows these probes to move.
				// the next update step will push the probe as far away as possible.
				// 
				// an alternative approach would be to calculate the amount of distance the
				// probe can move to the front side to stay inside the allowed range and then
				// to use the smaller value of this distance and pGIMoveMinDistToSurface
				//vec3 direction = rayDirection + hitNormal * pGIMoveMinDistToSurface;
				vec3 direction = rayDirection + hitNormal * 0.05; //0.01;
				
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
	
	// here we deviate from the paper. the problem happens with closed indoor scenes like
	// hallway and room driven scenes. in this situation especially for higher cascades
	// the probes can score less than 25% hits outside but near geometry for example around
	// corners. the main problem here is that many rays miss geometry at all.
	// 
	// we though do not want to turn assumeInGeometry on for situations where there are many
	// misses and little backface hits to avoid problems with probes in an ouitside scene
	// over a flat plane.
	// 
	// we use another bool flag to determine if we want to use front or back face offsets
	// using a modified condition. assumeInGeometry is the first condition and works as in
	// the regular case. as a second condition we use backface offset also at least one
	// backface is hit and the front face count is less than 5%. this is similar to saying
	// we have a large miss count
	bool useBackfaceOffsets = assumeInGeometry
		|| ( backfaceCount > 0.5 && frontfaceCount / float( pGIRaysPerProbe ) < 0.05 );
	
	if( useBackfaceOffsets ){
		// closest backface found and more than 25% of hits are backface hits. as a first step
		// we want to move to the other side of the face to end up at a minimum distance from
		// the surface. if this is not possible do not move and disable the probe
		fbOffset.xyz = closestBackOffset;
		
	}else if( countOffsets > 0.5 ){
		// front faces are hit
		fbOffset.xyz /= countOffsets;
		
		// averaging the offsets has the tendency to shorten the offset if multiple
		// rays contribute. reduce this effect by enlaring the offset with larger
		// counts. a base scaling is always applied to reduce the chance of moving
		// the probe again in the future
		//fbOffset.xyz *= 1.0 + 0.05 * ( countOffsets - 1 );
		fbOffset.xyz *= 1.0 + 0.05 * countOffsets;
	}
	
	// position used for ray tracing contains the previous update probe offset.
	// the previous offset has to be added to the new offset for it to be correct.
	// probeCoord is local. we need it though shifted
	fbOffset.xyz += prevOffset;
	
	uint flags = uint( pGIProbePosition[ gl_InstanceID ].w );
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
	// 
	// the original implementation uses a sphere range around the grid center. this though
	// causes probes not to move into hallways for higher cascades. this happens only with
	// box range which better uses the offset range available
	// 
	// as final change the calculation use depends on which offset is used. for frontface
	// offsets the sphere solution is better. for backface offset the box one
	vec3 gridOffset = fbOffset.xyz * moveMaxOffsetFactor;
	
	if( useBackfaceOffsets ){
		if( any( greaterThan( abs( gridOffset ), vec3( 1.0 ) ) ) ){
			// offset due to backface hit would move outside allowed range. do not move and
			// disable the probe. this also prevents future processing of the probe
			fbOffset.xyz = prevOffset;
			flags |= gipfDisabled;
		}
		
	}else{
		float gridOffsetLen = length( gridOffset );
		if( gridOffsetLen > 1.0 ){
			// offset due to frontface hit would move outside allowed range.
			// clamp the offset to the allowed range.
			if( gridOffsetLen > 0.001 ){
				fbOffset.xyz *= min( gridOffsetLen, 1.0 ) / gridOffsetLen;
			}
		}
	}
	
	// merge dynamic state
	if( ( flags & ( gipfNearGeometry | gipfDisabled ) ) == uint( 0 ) ){
		flags |= texelFetch( texState, texCoord, 0 ).r;
	}
	
	// write out flags
	fbOffset.w = float( flags );
	
	// debug
// 	fbOffset.xyz = vec4( 0.0, 0.0, 0.0, pGIProbePosition[ gl_InstanceID ].w );
	
	// copy to avoid stalling
	fbOffsetCopy = fbOffset;
}
