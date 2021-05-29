#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/probe_offset.glsl"
#include "v130/shared/defren/gi/probe_flags.glsl"

uniform ivec3 pGIGridCoordShift; // grid coordinate shift (wrapping around)
uniform ivec3 pParams; // probeSize, spaceSize, groupSpaceSize
uniform vec4 pPlaneLeft;
uniform vec4 pPlaneRight;
uniform vec4 pPlaneTop;
uniform vec4 pPlaneBottom;
uniform vec4 pPlaneNear;

in vec2 vTC;

out vec3 outColor;

ivec3 giGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pGIGridCoordShift ) % pGIGridProbeCount;
}

int giCoordToIndex( in ivec3 coord ){
	return pGIGridProbeCount.x * pGIGridProbeCount.z * coord.y + pGIGridProbeCount.x * coord.z + coord.x;
}


void main( void ){
	outColor = vec3( 0.0 );
	
	ivec2 tc = ivec2( vTC );
	ivec3 probeCoord;
	
	int probeStride = pParams.x + pParams.y;
	if( tc.y % probeStride >= pParams.x ){
		return;  // inside spacing
	}
	probeCoord.z = tc.y / probeStride;
	
	int groupSize = pParams.x * pGIGridProbeCount.z + pParams.y * ( pGIGridProbeCount.z - 1 );
	int groupStride = groupSize + pParams.z;
	if( tc.x % groupStride >= groupSize ){
		return;  // inside spacing
	}
	probeCoord.y = tc.x / groupStride;
	tc.x = tc.x % groupStride;
	
	if( tc.x % probeStride >= pParams.x ){
		return; // inside spacing
	}
	probeCoord.x = tc.x / probeStride;
	
	vec3 gridPosition = pGIGridProbeSpacing * vec3( probeCoord ) + pGIGridOrigin;
	bool insideView = ( dot( pPlaneNear.xyz, gridPosition ) >= pPlaneNear.w )
		&& ( dot( pPlaneLeft.xyz, gridPosition ) >= pPlaneLeft.w )
		&& ( dot( pPlaneRight.xyz, gridPosition ) >= pPlaneRight.w )
		&& ( dot( pPlaneTop.xyz, gridPosition ) >= pPlaneTop.w )
		&& ( dot( pPlaneBottom.xyz, gridPosition ) >= pPlaneBottom.w );
	
	probeCoord = giGridShiftToLocal( probeCoord );
	int index = giCoordToIndex( probeCoord );
	
	uint flags = gipoProbeFlags( probeCoord );
	bool disabled = ( flags & gipfDisabled ) == gipfDisabled;
	
	if( disabled ){
		if( insideView ){
			outColor = vec3( 0.15 );
			
		}else{
			outColor = vec3( 0.1 );
		}
		
	}else{
		if( insideView ){
			outColor = vec3( 0.1, 0.0, 0.0 );
			
		}else{
			outColor = vec3( 0.0, 0.1, 0.0 );
		}
	}
	
	int i;
	for( i=0; i<pGIProbeCount; i++ ){
		if( pGIProbeIndex[ i / 4 ][ i % 4 ] == index ){
			if( disabled ){
				if( insideView ){
					outColor = vec3( 0.25 );
					
				}else{
					outColor = vec3( 0.2 );
				}
				
			}else{
				if( insideView ){
					outColor = vec3( 1.0, 0.0, 0.0 );
					
				}else{
					outColor = vec3( 0.0, 1.0, 0.0 );
				}
			}
			break;
		}
	}
}
