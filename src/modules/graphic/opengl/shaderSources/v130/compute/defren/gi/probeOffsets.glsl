precision highp float;
precision highp int;

#include "v130/shared/uniform_const.glsl"
#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/probe_flags.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

#ifdef WITH_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache.glsl"
#endif


#ifdef WITH_RAY_CACHE
	layout(binding=2, r16f) uniform readonly image2DArray texCacheDistance;
	layout(binding=3, rgba8_snorm) uniform readonly image2DArray texCacheNormal;
#else
	layout(binding=0, rgba16f) uniform readonly image2D texPosition;
	layout(binding=1, rgba8_snorm) uniform readonly image2D texNormal;
#endif


struct sProbeOffset {
	vec3 offset;
	uint flags;
};

UBOLAYOUT_BIND(0) writeonly buffer ProbeOffset {
	sProbeOffset pProbeOffset[];
};

UBOLAYOUT_BIND(0) readonly buffer ProbeDynamicStates {
	uint pProbeDynamicStates[];
};


layout( local_size_x=64 ) in;


ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIGridCoordUnshift ) % pGIGridProbeCount;
}


struct sRayData{
	#ifdef WITH_RAY_CACHE
		vec3 rayDirection;
		float rayDistance;
	#else
		vec4 rayPosition; // position, distance
		
		vec3 rayDirection;
	#endif
	
	vec3 hitNormal;
	float distToSurface;
	
	vec3 moveProbe1;
	vec3 moveProbe2;
	vec3 moveProbe3;
};

shared sRayData vRayData[ 64 ];  // 5120, 6144


void main( void ){
	int index = int( gl_WorkGroupID.x );
	
	
	// parameters
	UFCONST vec3 nearGeometryRange = pGIGridProbeSpacing + pGIMoveMaxOffset * 2;
	
	vec3 probePosition = pGIProbePosition[ index ].xyz;
	
	ivec3 probeCoord = probeIndexToGridCoord( giTraceProbeProbeIndex( index ) );
	
	#ifdef WITH_RAY_CACHE
		ivec2 rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( index ) );
	#else
		ivec2 rayOffset = ivec2( ( index % pGIProbesPerLine ) * pGIRaysPerProbe, index / pGIProbesPerLine );
	#endif
	
	
	// calculate probe offset
	float closestFrontfaceDistance = 10000;
	float frontfaceCount = 0;
	float backfaceCount = 0;
	float countOffsets = 0;
	
	vec3 probeOffset = vec3( 0 );
	
	// position used for ray tracing contains the previous update probe offset.
	// the previous offset has to be added to the new offset for it to be correct.
	// probeCoord is local. we need it though shifted
	vec3 gridPosition = pGIGridProbeSpacing * vec3( giGridLocalToShift( probeCoord ) ) + pGIGridOrigin;
	vec3 prevOffset = probePosition - gridPosition;
	
	// max offset test scaling factor
	UFCONST vec3 moveMaxOffsetFactor = vec3( 1 ) / pGIMoveMaxOffset;
	
	// if we apply the offset of all back faces we end up in troubles. for example if
	// two back faces face each other the offset towards the other side of both faces
	// cancels out causing the probe to not move at all. what we do here is finding the
	// closest back face and applying the offset only if it is the closest.
	float closestBackDistance = 10000;
	vec3 closestBackOffset = vec3( 10000 ); // very large to force disable if no good hit found
	
	UFCONST int rayGroupCount = ( pGIRaysPerProbe - 1 ) / 64 + 1;
	int rg, i;
	
	for( rg=0; rg<rayGroupCount; rg++ ){
		int rayFirst = 64 * rg;
		
		// cooperative processing
		int rayIndex = rayFirst + int( gl_LocalInvocationIndex );
		
		barrier();
		
		if( rayIndex < pGIRaysPerProbe ){
			#ifdef WITH_RAY_CACHE
				ivec3 rayTC = ivec3( rayOffset + ivec2( rayIndex, 0 ), pGICascade );
				vRayData[ gl_LocalInvocationIndex ].rayDistance = imageLoad( texCacheDistance, rayTC ).r;
				vRayData[ gl_LocalInvocationIndex ].hitNormal = imageLoad( texCacheNormal, rayTC ).xyz;
				
				vRayData[ gl_LocalInvocationIndex ].rayDirection = pGIRayDirection[ gl_LocalInvocationIndex ]
					* vRayData[ gl_LocalInvocationIndex ].rayDistance;
				
				#define RAY_DISTANCE vRayData[ gl_LocalInvocationIndex ].rayDistance
				
			#else
				ivec2 rayTC = rayOffset + ivec2( rayIndex, 0 );
				vRayData[ gl_LocalInvocationIndex ].rayPosition = imageLoad( texPosition, rayTC ); // position, distance
				vRayData[ gl_LocalInvocationIndex ].hitNormal = imageLoad( texNormal, rayTC ).xyz;
				
				vRayData[ gl_LocalInvocationIndex ].rayDirection =
					vRayData[ gl_LocalInvocationIndex ].rayPosition.xyz - probePosition;
				
				#define RAY_DISTANCE vRayData[ gl_LocalInvocationIndex ].rayPosition.w
			#endif
			
			vRayData[ gl_LocalInvocationIndex ].distToSurface = dot( vRayData[ gl_LocalInvocationIndex ].hitNormal,
				vRayData[ gl_LocalInvocationIndex ].rayDirection );
			
			vRayData[ gl_LocalInvocationIndex ].moveProbe1 =
				vRayData[ gl_LocalInvocationIndex ].hitNormal * pGIMoveMinDistToSurface;
			
			vRayData[ gl_LocalInvocationIndex ].moveProbe2 = vRayData[ gl_LocalInvocationIndex ].rayDirection
				* ( 1.0 - pGIMoveMinDistToSurface / RAY_DISTANCE );
			
			vRayData[ gl_LocalInvocationIndex ].moveProbe3 = vRayData[ gl_LocalInvocationIndex ].rayDirection
				+ vRayData[ gl_LocalInvocationIndex ].hitNormal * 0.05; //0.01;
		}
		
		barrier();
		
		
		// per invocation processing
		int rayLimit = min( 64, pGIRaysPerProbe - rayFirst );
		
		for( i=0; i<rayLimit; i++ ){
			if( RAY_DISTANCE >= 9999 ){
				continue; // ray misses. do not move
			}
			
			if( vRayData[ i ].distToSurface < 0 ){
				frontfaceCount += 1;
				closestFrontfaceDistance = min( closestFrontfaceDistance, RAY_DISTANCE );
				
				if( RAY_DISTANCE >= pGIMoveMinDistToSurface ){
					continue; // far enough to have no influence
					
				}else if( RAY_DISTANCE < 0.001 ){
					// at surface. move along normal
					probeOffset += vRayData[ i ].moveProbe1;
					
				}else{
					// move back on ray until pGIMoveMinDistToSurface distance
					// offset = rayDirection - normalize(rayDirection) * pGIMoveMinDistToSurface
					// offset = normalize(rayDirection) * (rayLength - pGIMoveMinDistToSurface)
					// offset = rayDirection / rayLength * (rayLength - pGIMoveMinDistToSurface)
					// offset = rayDirection * ((rayLength - pGIMoveMinDistToSurface) / rayLength)
					probeOffset += vRayData[ i ].moveProbe2;
				}
				
			}else{
				backfaceCount += 1;
				
				if( RAY_DISTANCE < closestBackDistance ){
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
					
					if( length( ( prevOffset + vRayData[ i ].moveProbe3 ) * moveMaxOffsetFactor ) < 1 ){
						// consider backface ray only if not leaving allowed area
						closestBackOffset = vRayData[ i ].moveProbe3;
						closestBackDistance = RAY_DISTANCE;
					}
				}
			}
			
			countOffsets += 1;
		}
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
		probeOffset = closestBackOffset;
		
	}else if( countOffsets > 0.5 ){
		// front faces are hit
		probeOffset /= countOffsets;
		
		// averaging the offsets has the tendency to shorten the offset if multiple
		// rays contribute. reduce this effect by enlaring the offset with larger
		// counts. a base scaling is always applied to reduce the chance of moving
		// the probe again in the future
		//probeOffset *= 1 + 0.05 * ( countOffsets - 1 );
		probeOffset *= 1 + 0.05 * countOffsets;
	}
	
	// position used for ray tracing contains the previous update probe offset.
	// the previous offset has to be added to the new offset for it to be correct.
	// probeCoord is local. we need it though shifted
	probeOffset += prevOffset;
	
	uint probeFlags = uint( pGIProbePosition[ index ].w );
	probeFlags &= ~( gipfDisabled | gipfDynamicDisable );
	
	// in the original source code "spacing * 2 * 1.45" is used. but I think only
	// "spacing * (1 + 0.45 * 2)" aka "spacing * 1.9" is required. the thinking is this:
	// if a surface is farther away than "spacing" then it falls into the next cell and this
	// probe has no effect on it anymore no matter if by direct lighting nor ray lighting.
	// offset can be at most 0.45 . so if both probes are have maximum offset but in opposite
	// direction then this yields "1 + 0.45 + 0.45" times the spacing which is "spacing * 1.9"
	if( all( lessThanEqual( vec3( closestFrontfaceDistance ), nearGeometryRange ) ) && ! assumeInGeometry ) {
		probeFlags |= gipfNearGeometry;
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
	vec3 gridOffset = probeOffset * moveMaxOffsetFactor;
	
	if( useBackfaceOffsets ){
		if( any( greaterThan( abs( gridOffset ), vec3( 1 ) ) ) ){
			// offset due to backface hit would move outside allowed range. do not move and
			// disable the probe. this also prevents future processing of the probe
			probeOffset = prevOffset;
			probeFlags |= gipfDisabled;
		}
		
	}else{
		float gridOffsetLen = length( gridOffset );
		if( gridOffsetLen > 1 ){
			// offset due to frontface hit would move outside allowed range.
			// clamp the offset to the allowed range.
			if( gridOffsetLen > 0.001 ){
				probeOffset *= min( gridOffsetLen, 1 ) / gridOffsetLen;
			}
		}
	}
	
	// merge dynamic state
	if( ( probeFlags & ( gipfNearGeometry | gipfDisabled ) ) == uint( 0 ) ){
		probeFlags |= pProbeDynamicStates[ index ];
	}
	
	// debug
// 	probeOffset = vec3( 0 );
// 	probeFlags = uint( pGIProbePosition[ index ].w );
	
	// write probe parameters. this has to be done only one invocation
	if( gl_LocalInvocationIndex == 0 ){
		pProbeOffset[ index ].offset = probeOffset;
		pProbeOffset[ index ].flags = probeFlags;
	}
}
