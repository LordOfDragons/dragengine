// usage:
// #include "shared/interface/light/vertex.glsl"


#include "shared/interface/2d/vertex.glsl"


// light volume position (location 3)
// used by:
// - LayeredRendering != LayeredRenderingNone
#ifdef WITH_GEOMETRY_SHADER
	// opengl
	VARYING_BIND(3) out mediump vec3 vGSLightVolumePos;
	#define vLightVolumePos vGSLightVolumePos
	
#else
	// spir-v
	VARYING_BIND(3) out mediump vec3 vLightVolumePos;
#endif


// particle light color (location 4)
// used by:
// - LightMode == LightModeParticle
#ifdef WITH_SPIRV_OPENGL
	// spir-v
	VARYING_BIND(4) out mediump vec3 vParticleLightColor;
	
#else
	// opengl
	VARYING_BIND(4) out mediump vec3 vGSParticleLightColor;
	#define vParticleLightColor vGSParticleLightColor
#endif


// particle light range (location 5)
// used by:
// - LightMode == LightModeParticle
#ifdef WITH_SPIRV_OPENGL
	// spir-v
	VARYING_BIND(5) out mediump float vParticleLightRange;
	
#else
	// opengl
	VARYING_BIND(5) out mediump float vGSParticleLightRange;
	#define vParticleLightRange vGSParticleLightRange
#endif


// particle light position (location 6)
// used by:
// - LightMode == LightModeParticle
#ifdef WITH_SPIRV_OPENGL
	// spir-v
	VARYING_BIND(6) out mediump vec3 vParticleLightPosition;
	
#else
	// opengl
	VARYING_BIND(6) out mediump vec3 vGSParticleLightPosition;
	#define vParticleLightPosition vGSParticleLightPosition
#endif


// default outputs
void lightVertexShaderDefaultOutputs(){
	vertexShaderDefaultOutputs();
}
