precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

in vec3 inPosition;


ivec3 probeIndexToGridCoord( in int index ){
	return ivec3( index % pGIGridProbeCount.x, index / ( pGIGridProbeCount.x * pGIGridProbeCount.z ),
		( index % ( pGIGridProbeCount.x * pGIGridProbeCount.z ) ) / pGIGridProbeCount.x );
}


void main( void ){
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
	vec2 offset = vec2( groupOffset * mapProbeSizeBorder + 1, probeGrid.z * mapProbeSizeBorder + 1 );
	
	vec2 realSize = vec2( mapProbeSizeBorder ) * mapProbeScale;
	vec2 realOffset = offset * mapProbeScale * vec2( 2.0 ) + realSize + vec2( -1.0 );
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
