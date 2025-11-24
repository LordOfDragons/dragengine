// usage:
// #include "shared/interface/light/geometry.glsl"


#include "shared/interface/2d/geometry.glsl"


// light volume position (location 3)
// used by:
// - LayeredRendering != LayeredRenderingNone
VARYING_BIND(3) in mediump vec3 vGSLightVolumePos[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(3) out mediump vec3 vLightVolumePos;


// particle light color (location 4)
// used by:
// - LightMode == LightModeParticle
VARYING_BIND(4) in mediump vec3 vGSParticleLightColor[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(4) out mediump vec3 vParticleLightColor;


// particle light range (location 5)
// used by:
// - LightMode == LightModeParticle
VARYING_BIND(5) in mediump float vGSParticleLightRange[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(5) out mediump float vParticleLightRange;


// particle light position (location 6)
// used by:
// - LightMode == LightModeParticle
VARYING_BIND(6) in mediump vec3 vGSParticleLightPosition[GEOMETRY_SHADER_INPUT_SIZE];

VARYING_BIND(6) out mediump vec3 vParticleLightPosition;


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
