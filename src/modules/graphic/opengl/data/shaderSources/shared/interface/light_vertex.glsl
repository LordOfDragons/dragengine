// usage:
// #include "shared/interface/light_vertex.glsl"


#include "shared/interface/2d_vertex.glsl"


// light volume position (location 3)
// used by:
// - GSRenderStereo
// - GSLayer
#ifdef WITH_GEOMETRY_SHADER
	// opengl
	VARYING_BIND(3) out vec3 vGSLightVolumePos;
	#define vLightVolumePos vGSLightVolumePos
	
#else
	// spir-v
	VARYING_BIND(3) out vec3 vLightVolumePos;
#endif


// particle light color (location 4)
// used by:
// - LightMode == LightModeParticle
#ifdef WITH_SPIRV_OPENGL
	// spir-v
	VARYING_BIND(4) out vec3 vParticleLightColor;
	
#else
	// opengl
	VARYING_BIND(4) out vec3 vGSParticleLightColor;
	#define vParticleLightColor vGSParticleLightColor
#endif


// particle light range (location 5)
// used by:
// - LightMode == LightModeParticle
#ifdef WITH_SPIRV_OPENGL
	// spir-v
	VARYING_BIND(5) out float vParticleLightRange;
	
#else
	// opengl
	VARYING_BIND(5) out float vGSParticleLightRange;
	#define vParticleLightRange vGSParticleLightRange
#endif


// particle light position (location 6)
// used by:
// - LightMode == LightModeParticle
#ifdef WITH_SPIRV_OPENGL
	// spir-v
	VARYING_BIND(6) out vec3 vParticleLightPosition;
	
#else
	// opengl
	VARYING_BIND(6) out vec3 vGSParticleLightPosition;
	#define vParticleLightPosition vGSParticleLightPosition
#endif


// default outputs
void lightVertexShaderDefaultOutputs(){
	vertexShaderDefaultOutputs();
}
