// usage:
// #include "shared/interface/2d/geometry.glsl"


// configuration
#ifndef GEOMETRY_SHADER_INPUT_SIZE
	#define GEOMETRY_SHADER_INPUT_SIZE 3
#endif


// texture coordinates (location 0)
// used by:
// - LayeredRendering != LayeredRenderingNone
VARYING_BIND(0) in mediump vec2 vGSTexCoord[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(0) out mediump vec2 vTexCoord;


// screen coordinates (location 1)
// used by:
// - FullScreenQuad
VARYING_BIND(1) in mediump vec2 vGSScreenCoord[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(1) out mediump vec2 vScreenCoord;


// layer (location 2)
// used by:
// - LayeredRendering != LayeredRenderingNone
VARYING_BIND(2) flat in lowp int vGSLayer[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(2) flat out lowp int vLayer;


// default outputs
void geometryShaderDefaultOutputs(int index, int layer){
	vTexCoord = vGSTexCoord[index];
	vScreenCoord = vGSScreenCoord[index];
	vLayer = layer;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
}

void geometryShaderDefaultOutputs(int index){
	geometryShaderDefaultOutputs(index, vGSLayer[index]);
}
