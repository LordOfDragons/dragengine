precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/defines.glsl"


UBOLAYOUT_BIND(0) writeonly buffer Element {
	sElement pElement[];
};

UBOLAYOUT_BIND(1) readonly buffer UpdateElement {
	sElement pUpdateElement[];
};


layout( local_size_x=64 ) in;


void main( void ){
	if( gl_GlobalInvocationID.x >= pUpdateElementCount ){
		return;
	}
	
	uint s = gl_GlobalInvocationID.x;
	uint t = pUpdateElement[ s ].updateIndex;
	
	pElement[ t ].minExtend = pUpdateElement[ s ].minExtend;
	pElement[ t ].flags = pUpdateElement[ s ].flags;
	pElement[ t ].maxExtend = pUpdateElement[ s ].maxExtend;
	pElement[ t ].layerMask = pUpdateElement[ s ].layerMask;
}
