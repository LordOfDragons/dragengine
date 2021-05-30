precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/probe_flags.glsl"


uniform sampler2D texPosition;
uniform sampler2D texNormal;


flat in int vInstanceID;
flat in ivec3 vProbeCoord;

out uint outState;


ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIGridCoordUnshift ) % pGIGridProbeCount;
}

void main( void ){
	const vec3 nearGeometryRange = pGIGridProbeSpacing + pGIMoveMaxOffset * 2.0;
	ivec2 rayOffset = ivec2( ( vInstanceID % pGIProbesPerLine ) * pGIRaysPerProbe, vInstanceID / pGIProbesPerLine );
	vec3 probePosition = pGIProbePosition[ vInstanceID ].xyz;
	
	outState = uint( gipfDynamicDisable );
	
	int i;
	for( i=0; i<pGIRaysPerProbe; i++ ){
		ivec2 rayTC = rayOffset + ivec2( i, 0 );
		
		vec4 rayPosition = texelFetch( texPosition, rayTC, 0 ); // position, distance
		
		if( all( lessThanEqual( vec3( rayPosition.w ), nearGeometryRange ) )
		&& dot( texelFetch( texNormal, rayTC, 0 ).xyz, rayPosition.xyz - probePosition ) < 0.0 ){
			// front face hit inside required range
			outState = uint( 0 );
			return;
		}
	}
}
