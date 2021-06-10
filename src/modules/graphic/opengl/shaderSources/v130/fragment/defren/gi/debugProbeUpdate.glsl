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

#ifdef PASS2
	flat in ivec3 vProbeCoord;
	
#else
	in vec2 vTC;
#endif

out vec3 outColor;


ivec3 giGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pGIGridCoordShift ) % pGIGridProbeCount;
}

int giCoordToIndex( in ivec3 coord ){
	return pGIGridProbeCount.x * pGIGridProbeCount.z * coord.y + pGIGridProbeCount.x * coord.z + coord.x;
}


void main( void ){
	outColor = vec3( 0.0 );
	
	#ifdef PASS2
		ivec3 probeCoord = vProbeCoord;
		
	#else
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
	#endif
	
	vec3 gridPosition = pGIGridProbeSpacing * vec3( probeCoord ) + pGIGridOrigin;
	bool insideView = ( dot( pPlaneNear.xyz, gridPosition ) >= pPlaneNear.w )
		&& ( dot( pPlaneLeft.xyz, gridPosition ) >= pPlaneLeft.w )
		&& ( dot( pPlaneRight.xyz, gridPosition ) >= pPlaneRight.w )
		&& ( dot( pPlaneTop.xyz, gridPosition ) >= pPlaneTop.w )
		&& ( dot( pPlaneBottom.xyz, gridPosition ) >= pPlaneBottom.w );
	
	probeCoord = giGridShiftToLocal( probeCoord );
	int index = giCoordToIndex( probeCoord );
	
	uint flags = gipoProbeFlags( probeCoord, 0 ); // TODO 0=cascade
	bool disabled = ( flags & gipfDisabled ) == gipfDisabled;
	bool nearGeometry = ( flags & gipfNearGeometry ) == gipfNearGeometry;
	
	#ifdef PASS2
	bool updated = true;
	#else
	bool updated = false;
	#endif
	
	/*
	int i;
	for( i=0; i<pGIProbeCount; i++ ){
		if( pGIProbeIndex[ i / 4 ][ i % 4 ] == index ){
			updated = true;
			break;
		}
	}
	*/
	
	if( disabled ){
		outColor = mix( vec3( 0.5 ), vec3( 1 ), bvec3( insideView ) );
		
	}else if( nearGeometry ){
		outColor = mix( vec3( 0, 1, 0 ), vec3( 1, 0, 0 ), bvec3( insideView ) );
		
	}else{
		outColor = mix( vec3( 0.25, 0, 0.25 ), vec3( 0.5, 0, 0.5 ), bvec3( insideView ) );
	}
	
	if( ! updated ){
		outColor *= 0.1;
	}
}
