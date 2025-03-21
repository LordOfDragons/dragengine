precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_field.glsl"

const int pGIMaterialMapsPerRow = 32; // dummy
const int pGIMaterialMapSize = 64; // dummy

#include "shared/octahedral.glsl"
#include "shared/raycasting.glsl"


out float outResult;


void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	// calculate probe position. probes are spread out across the image according to their
	// index. the probe index is
	//   resolution.x * resolution.z * coord.y + resolution.x * coord.z + coord.x
	// the (0,0,0) coord is located at the pGIFieldOrigin location
	// 
	// the entire image is rendered for performance reasons but this includes probe indices
	// beyond the last valid probe index at the end. these are skiped
	ivec2 tc2 = tc / ivec2( pGIFieldProbeSize );
	int probeIndex = pGIFieldProbesPerLine * tc2.y + tc2.x;
	if( probeIndex >= pGIFieldProbeCount ){
		discard;
	}
	
	ivec3 probeCoord = ivec3( probeIndex % pGIFieldResolution.x,
		probeIndex / pGIFieldProbeStride,
		( probeIndex % pGIFieldProbeStride ) / pGIFieldResolution.x );
	
	vec3 position = pGIFieldOrigin + pGIFieldSpacing * vec3( probeCoord );
	
	// trace ray. the octahedral coordinates are used as normal
	vec3 direction = octahedralDecode( ( vec2( tc % ivec2( pGIFieldProbeSize ) )
		+ vec2( 0.5 ) ) * ( 2.0 / float( pGIFieldProbeSize ) ) - vec2( 1.0 ) );
	
	RayCastResult result;
	if( rayCastMesh( 0, 0, position, direction, result ) ){
		outResult = result.distance;
	}else{
		outResult = 10000.0;
	}
}
