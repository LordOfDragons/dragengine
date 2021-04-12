precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

in vec3 inPosition;

flat out ivec2 vRayOffset;
flat out ivec2 vOffset;
out float vBlendFactor;
flat out vec3 vProbePosition;


// NOTE findMSB exists since GLSL 4.0
#if __VERSION__ == 330 || __VERSION__ == 150 || __VERSION__ == 140 || __VERSION__ == 130
int findMSB( in int x ){
	int i;
	int mask;
	int res = -1;
	if( x < 0 ){
		x ~= x;
	}
	for( i=0; i<32; i++ ){
		mask = 0x80000000 >> i;
		if( x & mask ){
			res = 31 - i;
			break;
		}
	}
	return res;
}
#endif

ivec3 probeIndexToGridCoord( in int index ){
	return ivec3( index % pGIGridProbeCount.x, index / ( pGIGridProbeCount.x * pGIGridProbeCount.z ),
		( index % ( pGIGridProbeCount.x * pGIGridProbeCount.z ) ) / pGIGridProbeCount.x );
	/*
	return ivec3( index & ( pGIGridProbeCount.x - 1 ),
		index >> findMSB( pGIGridProbeCount.x * pGIGridProbeCount.z ),
		( index & ( ( pGIGridProbeCount.x * pGIGridProbeCount.z ) - 1 ) ) >> findMSB( pGIGridProbeCount.x ) );
	*/
}


void main( void ){
	int flags = int( pGIProbePosition[ gl_InstanceID ].w );
	
	vRayOffset.x = ( gl_InstanceID % pGIProbesPerLine ) * pGIRaysPerProbe;
	vRayOffset.y = gl_InstanceID / pGIProbesPerLine;
	vBlendFactor = ( flags & 1 ) == 1 ? pGIBlendUpdateProbe : 1.0f;
	vProbePosition = pGIProbePosition[ gl_InstanceID ].xyz;
	
	int probeIndex = pGIProbeIndex[ gl_InstanceID >> 2 ][ gl_InstanceID & 3 ]; // 4 IDs per array entry
	ivec3 probeGrid = probeIndexToGridCoord( probeIndex );
	
	// map layout: (probeCount.x * probeCount.y) x pGIGridProbeCount.z
	#ifdef MAP_IRRADIANCE
		int mapProbeSizeBorder = pGIIrradianceMapSize + 2;
		#define mapProbeScale pGIIrradianceMapScale
	#else
		int mapProbeSizeBorder = pGIDistanceMapSize + 2;
		#define mapProbeScale pGIDistanceMapScale
	#endif
	int groupOffset = pGIGridProbeCount.x * probeGrid.y + probeGrid.x;
	vOffset.x = groupOffset * mapProbeSizeBorder + 1;
	vOffset.y = probeGrid.z * mapProbeSizeBorder + 1;
	
	vec2 realSize = vec2( mapProbeSizeBorder ) * mapProbeScale;
	vec2 realOffset = vec2( vOffset ) * mapProbeScale * vec2( 2.0 ) + realSize + vec2( -1.0 );
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
