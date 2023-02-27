precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"


uniform uint pCount;


UBOLAYOUT_BIND(0) writeonly buffer ElementCullResult {
	uvec4 pElementCullResult[];
};


layout( local_size_x=64 ) in;


void main( void ){
	if( gl_GlobalInvocationID.x < pCount ){
		pElementCullResult[ gl_GlobalInvocationID.x ] = uvec4( 0 );
	}
}
