#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/probe_flags.glsl"

// WithRayCache
#include "shared/defren/gi/raycast/ray_cache.glsl"
#include "shared/defren/gi/trace_probe.glsl"


layout(binding=0) uniform sampler2D texPosition;
layout(binding=1) uniform sampler2D texNormal;


flat in int vInstanceID;
flat in ivec3 vProbeCoord;

out vec4 outOffset; // offset(xyz), flags(w)


ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIGridCoordUnshift ) % pGIGridProbeCount;
}

void main( void ){
	ivec2 rayOffset;
	if(WithRayCache){
		rayOffset = giRayCastCacheFirstTCFromProbeIndex( giTraceProbeProbeIndex( vInstanceID ) );
	}else{
		rayOffset = ivec2( ( vInstanceID % pGIProbesPerLine ) * pGIRaysPerProbe, vInstanceID / pGIProbesPerLine );
	}
	
	vec3 probePosition = pGIProbePosition[ vInstanceID ].xyz;
	uint flags = uint( pGIProbePosition[ vInstanceID ].w );
	
	const vec3 outerBounds = normalize( pGIGridProbeSpacing ) * ( length( pGIGridProbeSpacing ) + pGISelfShadowBias * 2.0 );
	
	vec4 closestBackface = vec4( 0.0, 0.0, 0.0, 10000.0 );
	vec4 closestFrontface = vec4( 0.0, 0.0, 0.0, 10000.0 );
	vec4 farthestFrontface = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	int backfaceCount = 0;
	int i;
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		ivec2 rayTC = rayOffset + ivec2( i, 0 );
		
		float rayDistance;
		vec4 rayPosition;
		if(WithRayCache){
			float rayDistance = texelFetch( texPosition, rayTC, 0 ).r;
		}else{
			rayPosition = texelFetch( texPosition, rayTC, 0 ); // position, distance
			rayDistance = rayPosition.w;
		}
		
		if( rayDistance >= 9999.0 ){
			continue; // ray misses
		}
		
		vec3 rayDirection, normRayDir;
		if(WithRayCache){
			rayDirection = pGIRayDirection[ i ] * rayDistance;
			normRayDir = pGIRayDirection[ i ];
		}else{
			rayDirection = rayPosition.xyz - probePosition;
			normRayDir = normalize( rayDirection );
		}
		
		vec3 hitNormal = texelFetch( texNormal, rayTC, 0 ).xyz; // only valid if ray does not miss
		float dotFacing = dot( hitNormal, normRayDir );
		
		if( dotFacing < 0.0 ){
			if( all( lessThan( abs( rayDirection ), outerBounds ) ) ){
				// static surface shaded by probe. Make it "just vigilant".
// 				flag = 4;
			}
			
			if( rayDistance < closestFrontface.w ){
				closestFrontface = vec4( normRayDir, rayDistance );
				
			}else if( rayDistance > farthestFrontface.w ){
				farthestFrontface = vec4( normRayDir, rayDistance );
			}
			
		}else{
			backfaceCount++;
			
			if( rayDistance < closestBackface.w ){
// 				closestBackface = vec4( normRayDir, rayDistance ); // paper version
				closestBackface = vec4( rayDirection + hitNormal * pGIMoveMinDistToSurface, rayDistance );
			}
		}
	}
	
	// position used for ray tracing contains the previous update probe offset. the previous
	// offset thus can be calculated this way. vProbeCoord is local and has to be shifted
	vec3 gridPosition = pGIGridProbeSpacing * vec3( giGridLocalToShift( vProbeCoord ) ) + pGIGridOrigin;
	outOffset = vec4( probePosition - gridPosition, flags );
	
	vec3 fullOffset = vec3( 10000.0 );
	
	// if closest backface is hit and more than 25% backfaces are hit consider inside geometry
	if( closestBackface.w < 9999.0 && float( backfaceCount ) / pGIRaysPerProbe > 0.25 ){
		// disable probe
// 		flag = 0;
		
		/*
		// paper version. can cause div-by-zero problems as well as probes not moving
		// out of the walls but shrugging along them
		
		// solve for the maximum scaling possible on each axis
		vec3 direction = closestBackface.xyz * closestBackface.w;
		vec3 positiveOffset = ( -outOffset.xyz + pGIMoveMaxOffset ) / direction;
		vec3 negativeOffset = ( -outOffset.xyz - pGIMoveMaxOffset ) / direction;
		vec3 combinedOffset = max( positiveOffset, negativeOffset );
		
		// slightly bias this point to ensure we stay within bounds
		const float epsilon = 1e-3;
		float scaleFactor = min( min( combinedOffset.x, combinedOffset.y ), combinedOffset.z ) - epsilon;
		
		// if we can not move through the backface do not move at all
		fullOffset = outOffset.xyz + direction * ( scaleFactor > 1.0 ? scaleFactor : 0.0 );
		*/
		
		fullOffset = outOffset.xyz + closestBackface.xyz;
		
	}else if( closestFrontface.w > 9999.0 ){
		// probe sees only backfaces and sky. disable probe
// 		flag = 0;
		
	}else if( closestFrontface.w < 0.05 ){
		// we hit no backfaces and a close frontface (within 2 cm). set to "Newly Vigilant"
// 		flag = 4;
		
		// ensure that we never move through the farthest frontface.
		// move minimum distance to ensure not moving on a future iteration
		vec3 direction = farthestFrontface.xyz * min( farthestFrontface.w, 0.2 );
		
		// farthest frontface may also be the closest if the probe can only
		// see one surface. if this is the case, don't move the probe
		if( dot( direction, closestFrontface.xyz ) <= 0.5 ){
			fullOffset = outOffset.xyz + direction;
		}
	}
	
	// clamp maximum distance to pGIMoveMaxOffset to avoid degenerate cases
	if( all( lessThan( abs( fullOffset ), pGIMoveMaxOffset ) ) ){
		// we are going to move. set probe uninitialized
// 		flag = 6;
		outOffset.xyz = fullOffset;
	}
	
// 	outOffset.xyz = vec3(0); // DEBUG
}


/*
paper notes. flags found in the code:
- 0: disabled
- 1: sleep
- 2: newly awake
- 3: awake
- 4: newly vigilant
- 5: vigilant
- 6: uninitialized, moving
*/
