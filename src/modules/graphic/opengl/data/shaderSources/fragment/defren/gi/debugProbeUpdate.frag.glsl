#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_gi.glsl"

uniform int pGIDebugCascade;
uniform ivec3 pParams; // probeSize, spaceSize, groupSpaceSize
uniform vec4 pPlaneLeft;
uniform vec4 pPlaneRight;
uniform vec4 pPlaneTop;
uniform vec4 pPlaneBottom;
uniform vec4 pPlaneNear;

#define pGIGridProbeCount pGIParams[pGIDebugCascade].probeCount
#include "shared/defren/gi/probe_offset.glsl"
#include "shared/defren/gi/probe_flags.glsl"

#ifdef PASS2
	flat in ivec3 vProbeCoord;
	
#else
	in vec2 vTC;
#endif

layout(location=0) out vec3 outColor;


ivec3 giGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pGIParams[pGIDebugCascade].gridCoordShift ) % pGIParams[pGIDebugCascade].probeCount;
}

int giCoordToIndex( in ivec3 coord ){
	return pGIParams[pGIDebugCascade].probeCount.x * pGIParams[pGIDebugCascade].probeCount.z * coord.y
		+ pGIParams[pGIDebugCascade].probeCount.x * coord.z + coord.x;
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
		
		int groupSize = pParams.x * pGIParams[pGIDebugCascade].probeCount.z
			+ pParams.y * ( pGIParams[pGIDebugCascade].probeCount.z - 1 );
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
	
	vec3 gridPosition = pGIParams[pGIDebugCascade].probeSpacing * vec3( probeCoord )
		+ pGIParams[pGIDebugCascade].gridOrigin;
	bool insideView = ( dot( pPlaneNear.xyz, gridPosition ) >= pPlaneNear.w )
		&& ( dot( pPlaneLeft.xyz, gridPosition ) >= pPlaneLeft.w )
		&& ( dot( pPlaneRight.xyz, gridPosition ) >= pPlaneRight.w )
		&& ( dot( pPlaneTop.xyz, gridPosition ) >= pPlaneTop.w )
		&& ( dot( pPlaneBottom.xyz, gridPosition ) >= pPlaneBottom.w );
	
	probeCoord = giGridShiftToLocal( probeCoord );
	int index = giCoordToIndex( probeCoord );
	
	uint flags = gipoProbeFlags( probeCoord, pGIDebugCascade );
	bool disabled = ( flags & gipfDisabled ) == gipfDisabled;
	bool nearGeometry = ( flags & gipfNearGeometry ) == gipfNearGeometry;
	
	#ifdef PASS2
	bool updated = true;
	#else
	bool updated = false;
	#endif
	
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
