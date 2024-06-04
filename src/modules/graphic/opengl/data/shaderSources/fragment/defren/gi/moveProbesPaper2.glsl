precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/probe_flags.glsl"

#ifdef WITH_RAY_CACHE
	#include "shared/defren/gi/raycast/ray_cache.glsl"
	#include "shared/defren/gi/trace_probe.glsl"
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
	
	vec4 closestBackface = vec4( 0.0, 0.0, 0.0, 10000.0 );
	vec4 closestFrontface = vec4( 0.0, 0.0, 0.0, 10000.0 );
	vec4 farthestFrontface = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	float backfaceCount = 0.0;
	int i;
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		ivec2 rayTC = rayOffset + ivec2( i, 0 );
		
		#ifdef WITH_RAY_CACHE
			float rayDistance = texelFetch( texPosition, rayTC, 0 ).r;
		#else
			vec4 rayPosition = texelFetch( texPosition, rayTC, 0 ); // position, distance
			#define rayDistance rayPosition.w
		#endif
		
		if( rayDistance >= 9999.0 ){
			continue; // ray misses
		}
		
		#ifdef WITH_RAY_CACHE
			vec3 rayDirection = pGIRayDirection[ i ] * rayDistance;
			#define normRayDir pGIRayDirection[ i ]
		#else
			vec3 rayDirection = rayPosition.xyz - probePosition;
			vec3 normRayDir = normalize( rayDirection );
		#endif
		
		vec3 hitNormal = texelFetch( texNormal, rayTC, 0 ).xyz; // only valid if ray does not miss
		float dotFacing = dot( hitNormal, normRayDir );
		
		if( dotFacing <= 0.0 ){
			if( rayDistance < closestFrontface.w ){
				closestFrontface = vec4( normRayDir, rayDistance );
				
			}else if( rayDistance > farthestFrontface.w ){
				farthestFrontface = vec4( normRayDir, rayDistance );
			}
			
		}else{
			backfaceCount += 1.0;
			
			if( rayDistance < closestBackface.w ){
				closestBackface = vec4( normRayDir, rayDistance );
			}
		}
	}
	
	// position used for ray tracing contains the previous update probe offset. the previous
	// offset thus can be calculated this way. vProbeCoord is local and has to be shifted
	vec3 gridPosition = pGIGridProbeSpacing * vec3( giGridLocalToShift( vProbeCoord ) ) + pGIGridOrigin;
	outOffset = vec4( probePosition - gridPosition, 0.0 );
	
	vec3 fullOffset = vec3( 10000.0 );
	
	const float gridSpacing = length( pGIGridProbeSpacing );
	const float probeDistanceScale = 1.0;
		// ^= according to documentation start with 1 with first update step then move
		//    towards 0 on upcoming steps. so this would mean on step 0 use 1 and with
		//    each next step -0.2 so to get the chain: 1, 0.8, 0.6, 0.4, 0.2
	const float probeMinFrontfaceDistance = 1.0;
	
	// if closest backface is hit and more than 25% backfaces are hit consider inside geometry
	bool assumeInGeometry = backfaceCount / float( pGIRaysPerProbe ) > 0.25;
	
	if( closestBackface.w < 9999.0 && assumeInGeometry ){
		vec3 direction = closestBackface.xyz * closestBackface.w;
		
		fullOffset = outOffset.xyz + direction * ( probeDistanceScale + 1.0 );
		
	}else if( closestFrontface.w < gridSpacing * probeMinFrontfaceDistance ){
		// ^= in the revised paper RHS is 0.05
		
		// do not move probe if moving towards farthest frontface bring us closer to nearest frontface
		if( dot( closestFrontface.xyz, farthestFrontface.xyz ) <= 0.0 ){
			// ensure probe never moves through farthest frontface
			
			// NOTE in the original code this line makes no sense since farthestFrontface.w is
			//      never used below this point. reverting to revised paper version
// 			farthestFrontface.w *= min( farthestFrontface.w, 1.0 );
			vec3 direction = farthestFrontface.xyz * min( farthestFrontface.w, 0.2 );
			
			// probeDistanceScale decreases from 1 to 0 for backface movement.
			// it can go to 0 to ensure some movement away from close surfaces
			fullOffset = outOffset.xyz + direction * probeDistanceScale;
		}
		
	}else if( closestFrontface.w > gridSpacing * ( probeMinFrontfaceDistance + 1.0 ) ){
		// probe is not near anything. try move it back to zero offset
		// 
		// NOTE this branch is used for far away front face hits and misses
		float moveBackMargin = min( closestFrontface.w - gridSpacing * probeMinFrontfaceDistance, length( outOffset.xyz ) );
		vec3 moveBackDirection = normalize( -outOffset.xyz );
		fullOffset = outOffset.xyz + moveBackDirection * moveBackMargin;
	}
	
	// absolute maximum distance that probe could be moved should satisfy ellipsoid equation:
	// x^2 / probeGridSpacing.x^2 + y^2 / probeGridSpacing.y^2 + z^2 / probeGridSpacing.y^2 < (0.5)^2
	// c/ Clamp to less than maximum distance to avoid degenerate cases
	vec3 normalizedOffset = fullOffset / pGIGridProbeSpacing;
	if( dot( normalizedOffset, normalizedOffset ) < 0.2025f ){ // 0.45 * 0.45 == 0.2025
		outOffset.xyz = fullOffset;
	}
	
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
	
// 	outOffset.xyz = vec3(0); // DEBUG
}
