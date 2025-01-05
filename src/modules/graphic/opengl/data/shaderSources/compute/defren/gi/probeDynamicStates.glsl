precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/probe_flags.glsl"
#include "shared/defren/gi/trace_probe.glsl"


layout(binding=0, rgba16f) uniform readonly mediump image2D texPosition;
layout(binding=1, rgba8_snorm) uniform readonly mediump image2D texNormal;


UBOLAYOUT_BIND(0) writeonly restrict buffer ProbeDynamicStates {
	uint pProbeDynamicStates[];
};


layout( local_size_x=64 ) in;


ivec3 probeIndexToGridCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

shared ivec4 vFrontFaceCount[16];

void main( void ){
	int index = int( gl_WorkGroupID.x );
	
	UFCONST vec3 nearGeometryRange = pGIGridProbeSpacing + pGIMoveMaxOffset * 2.0;
	ivec2 rayOffset = ivec2( ( index % pGIProbesPerLine ) * pGIRaysPerProbe, index / pGIProbesPerLine );
	
	vec3 probePosition = vec3( pGIProbePosition[ index ] );
	
	// determine if front face hit
	UFCONST int rayGroupCount = ( pGIRaysPerProbe - 1 ) / 64 + 1;
	bool frontFaceHit = false;
	int rg;
	
	if( gl_LocalInvocationIndex == uint( 0 ) ){
		pProbeDynamicStates[ index ] = uint( gipfDynamicDisable );
	}
	
	for( rg=0; rg<rayGroupCount; rg++ ){
		int rayFirst = 64 * rg;
		
		// cooperative processing
		int rayIndex = rayFirst + int( gl_LocalInvocationIndex );
		
		if( rayIndex < pGIRaysPerProbe ){
			ivec2 rayTC = rayOffset + ivec2( rayIndex, 0 );
			
			vec4 rayPosition = imageLoad( texPosition, rayTC ); // position, distance
			
			vFrontFaceCount[ gl_LocalInvocationIndex / uint( 4 ) ][ gl_LocalInvocationIndex % uint( 4 ) ] =
				   all( lessThanEqual( vec3( rayPosition.w ), nearGeometryRange ) )
				&& dot( imageLoad( texNormal, rayTC ).xyz, rayPosition.xyz - probePosition ) < 0.0
				? 1 : 0;
		}
		barrier();
		
		// per invocation processing. combine all results by adding them
		ivec3 offset1 = ivec3( gl_LocalInvocationIndex ) * ivec3( 2, 4, 8 );
		ivec3 offset2 = offset1 + ivec3( 1, 2, 4 );
		
		// [0]+=[1], [2]+=[3], [4]+=[5], [6]+=[7], [8]+=[9], [10]+=[11], [12]+=[13], [14]+=[15]
		if( gl_LocalInvocationIndex < uint( 8 ) ){
			vFrontFaceCount[ offset1.x ] += vFrontFaceCount[ offset2.x ];
		}
		barrier();
		
		// [0]+=[2], [4]+=[6], [8]+=[10], [12]+=[14]
		if( gl_LocalInvocationIndex < uint( 4 ) ){
			vFrontFaceCount[ offset1.y ] += vFrontFaceCount[ offset2.y ];
		}
		barrier();
		
		// [0]+=[4], [8]+=[12]
		if( gl_LocalInvocationIndex < uint( 2 ) ){
			vFrontFaceCount[ offset1.z ] += vFrontFaceCount[ offset2.z ];
		}
		barrier();
		
		// [0]+=[8] and update frontFaceCount. we have to make sure all invocations exit
		// the shader but only 1 updates the buffer
		
		if(any(notEqual(vFrontFaceCount[0] + vFrontFaceCount[8], ivec4(0)))){
			// front face hit inside required range
			if( gl_LocalInvocationIndex == uint( 0 ) ){
				pProbeDynamicStates[ index ] = uint( 0 );
			}
			return;
		}
	}
}
