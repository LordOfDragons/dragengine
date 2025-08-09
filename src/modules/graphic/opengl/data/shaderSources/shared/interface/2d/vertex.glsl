// usage:
// #include "shared/interface/2d/vertex.glsl"


// texture coordinates (location 0)
// used by:
// - LayeredRendering != LayeredRenderingNone
#ifdef WITH_GEOMETRY_SHADER
	// opengl
	VARYING_BIND(0) out vec2 vGSTexCoord;
	#define vTexCoord vGSTexCoord
	
#else
	// spir-v
	VARYING_BIND(0) out vec2 vTexCoord;
#endif


// screen coordinates (location 1)
// used by:
// - FullScreenQuad
#ifdef WITH_GEOMETRY_SHADER
	// opengl
	VARYING_BIND(1) out vec2 vGSScreenCoord;
	#define vScreenCoord vGSScreenCoord
	
#else
	// spir-v
	VARYING_BIND(1) out vec2 vScreenCoord;
#endif


// layer (location 2)
// used by:
// - LayeredRendering != LayeredRenderingNone
#ifdef WITH_GEOMETRY_SHADER
	// opengl
	VARYING_BIND(2) flat out int vGSLayer;
	#define vLayer vGSLayer
	
#else
	// spir-v
	VARYING_BIND(2) flat out int vLayer;
#endif


// default outputs
void vertexShaderDefaultOutputs(){
	vLayer = 0;
}
