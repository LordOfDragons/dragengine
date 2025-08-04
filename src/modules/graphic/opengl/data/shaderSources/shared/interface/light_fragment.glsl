// usage:
// #include "shared/interface/light_fragment.glsl"


#include "shared/interface/2d_fragment.glsl"


// light volume position (location 3)
// used by:
// - GSRenderStereo
// - GSLayer
VARYING_BIND(3) in vec3 vLightVolumePos;


// particle light color (location 4)
// used by:
// - ParticleLight
VARYING_BIND(4) in vec3 vParticleLightColor;


// particle light range (location 5)
// used by:
// - ParticleLight
VARYING_BIND(5) in float vParticleLightRange;


// particle light position (location 6)
VARYING_BIND(6) in vec3 vParticleLightPosition;
