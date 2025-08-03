#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/plan/world_element_geometry.glsl"


SSBOLAYOUT(0) readonly buffer ClearGeometry {
	uint pClearGeometries[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};

UNIFORM_BIND(0) uniform uint pClearGeometryCount;


layout( local_size_x=64 ) in;


void main( void ){
	uint clearIndex = gl_GlobalInvocationID.x;
	if( clearIndex >= pClearGeometryCount ){
		return;
	}
	
	// this one is important to clear. all others can be neglected
	uint geometryIndex = pClearGeometries[ clearIndex ];
	
	pElementGeometries[ geometryIndex ].renderFilter = uint( 0 );
}
