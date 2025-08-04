// usage:
// #include "shared/interface/light_geometry.glsl"


#include "shared/interface/2d_geometry.glsl"


// light volume position (location 3)
// used by:
// - GSRenderStereo
// - GSLayer
VARYING_BIND(3) in vec3 vGSLightVolumePos[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(3) out vec3 vLightVolumePos;


// particle light color (location 4)
// used by:
// - ParticleLight
VARYING_BIND(4) in vec3 vGSParticleLightColor[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(4) out vec3 vParticleLightColor;


// particle light range (location 5)
// used by:
// - ParticleLight
VARYING_BIND(5) in float vGSParticleLightRange[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(5) out float vParticleLightRange;


// particle light position (out location 6)
// used by:
// - ParticleLight
VARYING_BIND(6) out vec3 vParticleLightPosition;


// default outputs
void lightGeometryShaderDefaultOutputs(int index, int layer){
	vLightVolumePos = vGSLightVolumePos[index];
	vParticleLightColor = vGSParticleLightColor[index];
	vParticleLightRange = vGSParticleLightRange[index];
	geometryShaderDefaultOutputs(index, layer);
}

void lightGeometryShaderDefaultOutputs(int index){
	lightGeometryShaderDefaultOutputs(index, vGSLayer[index]);
}
