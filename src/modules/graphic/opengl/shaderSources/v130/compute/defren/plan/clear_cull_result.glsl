precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"


uniform uint pElementCount;


UBOLAYOUT_BIND(0) writeonly buffer Element {
	sElement pElement[];
};


layout( local_size_x=64 ) in;


void main( void ){
	if( gl_GlobalInvocationID.x < pElementCount ){
		pElement[ gl_GlobalInvocationID.x ].cullResult = 0;
	}
}
