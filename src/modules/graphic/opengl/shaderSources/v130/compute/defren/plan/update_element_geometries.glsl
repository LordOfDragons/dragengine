precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/plan/world_element_geometry.glsl"


UBOLAYOUT_BIND(0) readonly buffer UpdateElementGeometry {
	sElementGeometry pUpdateElementGeometry[];
};

UBOLAYOUT_BIND(1) readonly buffer UpdateIndex {
	uvec4 pUpdateIndex[];
};

UBOLAYOUT_BIND(2) writeonly buffer ElementGeometry {
	sElementGeometry pElementGeometries[];
};

uniform uint pUpdateElementGeometryCount;


layout( local_size_x=64 ) in;


void main( void ){
	if( gl_GlobalInvocationID.x >= pUpdateElementGeometryCount ){
		return;
	}
	
	uint s = gl_GlobalInvocationID.x;
	uint t = pUpdateIndex[ s / uint( 4 ) ][ s % uint( 4 ) ];
	
	pElementGeometries[ t ].element = pUpdateElementGeometry[ s ].element;
	pElementGeometries[ t ].lod = pUpdateElementGeometry[ s ].lod;
	pElementGeometries[ t ].renderFilter = pUpdateElementGeometry[ s ].renderFilter;
	pElementGeometries[ t ].skinTexture = pUpdateElementGeometry[ s ].skinTexture;
	pElementGeometries[ t ].pipelineBase = pUpdateElementGeometry[ s ].pipelineBase;
	pElementGeometries[ t ].vao = pUpdateElementGeometry[ s ].vao;
	pElementGeometries[ t ].instance = pUpdateElementGeometry[ s ].instance;
	pElementGeometries[ t ].spbInstance = pUpdateElementGeometry[ s ].spbInstance;
	pElementGeometries[ t ].tucs = pUpdateElementGeometry[ s ].tucs;
}
