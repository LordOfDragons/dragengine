// usage:
// #include "shared/interface/2d_fragment.glsl"


// texture coordinates (location 0)
// used by:
// - GSRenderStereo
// - GSLayer
VARYING_BIND(0) in vec2 vTexCoord;


// screen coordinates (location 1)
// used by:
// - FullScreenQuad
VARYING_BIND(1) in vec2 vScreenCoord;


// layer (location 2)
// used by:
// - VSRenderStereo
// - GSRenderStereo
// - GSLayer
VARYING_BIND(2) flat in int vLayer;
