#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/ubo_clear.glsl"
#include "shared/image_buffer.glsl"


// MapIrradiance
layout(binding=0, rgba16f) uniform writeonly restrict HIGHP image2DArray texProbeIrradiance;

// !MapIrradiance
layout(binding=0, IMG_RG16F_FMT) uniform writeonly restrict HIGHP IMG_RG16F_2DARR texProbeDistance;


// Irradiance: 10x10, Distance: 18x18 (size + 1 pixel border)
CT_COMPUTE_IN_SIZE_XY


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
	int mapProbeSize = MapIrradiance ? pGIIrradianceMapSize : pGIDistanceMapSize;
	
	ivec3 probeGrid = probeIndexToGridCoord( index );
	ivec2 tcProbe = ivec2( pGIGridProbeCount.x * probeGrid.y + probeGrid.x, probeGrid.z ) * ( mapProbeSize + 2 ) + ivec2( 1 );
	ivec2 tcLocal = ivec2( gl_LocalInvocationID );
	ivec3 tcClear = ivec3( tcProbe + tcLocal, pGICascade );
	
	if(MapIrradiance){
		imageStore(texProbeIrradiance, tcClear, vec4(0, 0, 0, 1));
	}else{
		imageStore(texProbeDistance, tcClear, IMG_RG16F_STORE(vec2(
			pGIMaxProbeDistance, pGIMaxProbeDistance * pGIMaxProbeDistance)));
	}
}
