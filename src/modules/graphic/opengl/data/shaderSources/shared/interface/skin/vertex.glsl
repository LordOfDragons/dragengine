// usage:
// #include "shared/interface/skin/vertex.glsl"

#ifdef WITH_TESSELLATION_SHADER
	// opengl
	const bool PassOnNextStage = true;
	#include "shared/interface/skin/out_tessellation_control.glsl"
	
#elif defined WITH_GEOMETRY_SHADER
	// opengl
	const bool PassOnNextStage = true;
	#include "shared/interface/skin/out_geometry.glsl"
	
#else
	// spir-v
	const bool PassOnNextStage = WithGeometryShader || WithTessellationShader;
	#include "shared/interface/skin/out_fragment.glsl"
#endif

#include "shared/interface/skin/out_redirect.glsl"


// default outputs
void vertexShaderDefaultOutputs(){
	vLayer = 0;
}


// VSRenderLayer
UNIFORM_BIND(1) uniform int pDrawIDOffset;


int getInLayer(){
	#ifdef SUPPORTS_VSDRAWPARAM
	return gl_DrawID + pDrawIDOffset;
	#else
	return 0;
	#endif
}
#define inLayer getInLayer()
