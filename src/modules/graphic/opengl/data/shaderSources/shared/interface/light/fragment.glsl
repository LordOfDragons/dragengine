// usage:
// #include "shared/interface/light/fragment.glsl"


#include "shared/interface/2d/fragment.glsl"


// light volume position (location 3)
// used by:
// - LayeredRendering != LayeredRenderingNone
VARYING_BIND(3) in vec3 vLightVolumePos;


// particle light color (location 4)
// used by:
// - LightMode == LightModeParticle
VARYING_BIND(4) in vec3 vParticleLightColor;


// particle light range (location 5)
// used by:
// - LightMode == LightModeParticle
VARYING_BIND(5) in float vParticleLightRange;


// particle light position (location 6)
VARYING_BIND(6) in vec3 vParticleLightPosition;
