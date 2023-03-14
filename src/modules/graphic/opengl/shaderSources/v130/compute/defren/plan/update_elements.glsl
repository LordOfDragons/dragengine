precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element.glsl"


UBOLAYOUT_BIND(0) readonly buffer UpdateElement {
	sElement pUpdateElement[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer Element {
	sElement pElement[];
};

uniform uint pUpdateElementCount;


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
	pElement[ t ].firstGeometry = pUpdateElement[ s ].firstGeometry;
	pElement[ t ].geometryCount = pUpdateElement[ s ].geometryCount;
	pElement[ t ].lodFactors = pUpdateElement[ s ].lodFactors;
	pElement[ t ].highestLod = pUpdateElement[ s ].highestLod;
}
