precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/occtracing.glsl"

in vec3 inPosition;

out ivec2 vRayOffset;
out ivec2 vOffset;
out vec3 vProbePosition;


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
	return ivec3( index % pGridProbeCount.x, index / ( pGridProbeCount.x * pGridProbeCount.z ),
		( index % ( pGridProbeCount.x * pGridProbeCount.z ) ) / pGridProbeCount.x );
	/*
	return ivec3( index & ( pGridProbeCount.x - 1 ),
		index >> findMSB( pGridProbeCount.x * pGridProbeCount.z ),
		( index & ( ( pGridProbeCount.x * pGridProbeCount.z ) - 1 ) ) >> findMSB( pGridProbeCount.x ) );
	*/
}


void main( void ){
	vRayOffset.x = ( gl_InstanceID % pProbesPerLine ) * pRaysPerProbe;
	vRayOffset.y = gl_InstanceID / pProbesPerLine;
	vProbePosition = pProbePosition[ gl_InstanceID ];
	
	int probeIndex = pProbeIndex[ gl_InstanceID >> 2 ][ gl_InstanceID & 3 ]; // 4 IDs per array entry
	ivec3 probeGrid = probeIndexToGridCoord( probeIndex );
	
	// map layout: (probeCount.x * probeCount.y) x pGridProbeCount.z
	#ifdef MAP_OCCLUSION
		int mapProbeSizeBorder = pOcclusionMapSize + 2;
		#define mapProbeScale pOcclusionMapScale
	#else
		int mapProbeSizeBorder = pDistanceMapSize + 2;
		#define mapProbeScale pDistanceMapScale
	#endif
	int groupOffset = pGridProbeCount.x * probeGrid.y + probeGrid.x;
	vOffset.x = groupOffset * mapProbeSizeBorder + 1;
	vOffset.y = probeGrid.z * mapProbeSizeBorder + 1;
	
	vec2 realSize = vec2( mapProbeSizeBorder ) * mapProbeScale;
	vec2 realOffset = vec2( vOffset ) * mapProbeScale * vec2( 2.0 ) + realSize + vec2( -1.0 );
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
