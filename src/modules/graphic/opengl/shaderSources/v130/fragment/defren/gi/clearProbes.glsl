precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

UBOLAYOUT uniform GIClearParameters{
	uvec4 pGIClearProbes[ GI_CLEAR_PROBES_COUNT ];
};

out vec4 outValue;


#ifdef MAP_IRRADIANCE
	#define mapProbeSize pGIIrradianceMapSize
#else
	#define mapProbeSize pGIDistanceMapSize
#endif

ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}


void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy ) - ivec2( 1 ); // border around entire image
	
	#ifdef MAP_IRRADIANCE
		tc = tc / ivec2( pGIIrradianceMapSize + 2 );
	#else
		tc = tc / ivec2( pGIDistanceMapSize + 2 );
	#endif
	
	// discard if inside image border (or rather outside probes including their border)
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	
	if( any( lessThan( tc, ivec2( 0 ) ) )
	|| any( greaterThanEqual( tc, ivec2( stride, pGIGridProbeCount.z ) ) ) ){
		discard;
	}
	
	// determine if probe has to be cleared. pGIClearProbes contains 32 probe 1-bit flags
	// per component. "/ 128" comes from "/ 32 / 4"
	ivec3 probeCoord = ivec3( tc.x % pGIGridProbeCount.x, tc.x / pGIGridProbeCount.x, tc.y );
	int probeIndex = stride * probeCoord.y + pGIGridProbeCount.x * probeCoord.z + probeCoord.x;
	
	if( ( pGIClearProbes[ probeIndex / 128 ][ ( probeIndex % 128 ) / 32 ]
	& ( uint( 1 ) << ( probeIndex % 32 ) ) ) == uint( 0 ) ){
		discard;
	}
	
	// clear probe
	#ifdef MAP_IRRADIANCE
		outValue = vec4( 0, 0, 0, 1 );
	#else
		outValue = vec4( 4, 16, 0, 1 );
	#endif
}
