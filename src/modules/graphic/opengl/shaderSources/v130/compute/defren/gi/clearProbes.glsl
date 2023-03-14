precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/ubo_clear.glsl"


#ifdef MAP_IRRADIANCE
	layout(binding=0, rgba16f) uniform writeonly restrict image2DArray texProbe;
#else
	layout(binding=0, rg16f) uniform writeonly restrict image2DArray texProbe;
#endif


#ifdef MAP_IRRADIANCE
	layout( local_size_x=10, local_size_y=10 ) in; // 8x8 + 1 pixel border
#else
	layout( local_size_x=18, local_size_y=18 ) in; // 16x16 + 1 pixel border
#endif


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
	int index = int( gl_WorkGroupID.x );
	
	// determine if probe has to be cleared. pGIClearProbes contains 32 probe 1-bit flags
	// per component. "/ 128" comes from "/ 32 / 4"
	if( ( pGIClearProbes[ index / 128 ][ ( index % 128 ) / 32 ] & ( uint( 1 ) << ( index % 32 ) ) ) == uint( 0 ) ){
		return;
	}
	
	// clear probe
	ivec3 probeGrid = probeIndexToGridCoord( index );
	ivec2 tcProbe = ivec2( pGIGridProbeCount.x * probeGrid.y + probeGrid.x, probeGrid.z ) * ( mapProbeSize + 2 ) + ivec2( 1 );
	ivec2 tcLocal = ivec2( gl_LocalInvocationID );
	ivec3 tcClear = ivec3( tcProbe + tcLocal, pGICascade );
	
	#ifdef MAP_IRRADIANCE
		imageStore( texProbe, tcClear, vec4( 0, 0, 0, 1 ) );
	#else
		imageStore( texProbe, tcClear, vec4( pGIMaxProbeDistance, pGIMaxProbeDistance * pGIMaxProbeDistance, 0, 1 ) );
	#endif
}
