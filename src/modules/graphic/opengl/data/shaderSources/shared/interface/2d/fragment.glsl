// usage:
// #include "shared/interface/2d/fragment.glsl"


// texture coordinates (location 0)
// used by:
// - LayeredRendering != LayeredRenderingNone
VARYING_BIND(0) in mediump vec2 vTexCoord;


// screen coordinates (location 1)
// used by:
// - FullScreenQuad
VARYING_BIND(1) in mediump vec2 vScreenCoord;


// layer (location 2)
// used by:
// - LayeredRendering != LayeredRenderingNone
VARYING_BIND(2) flat in lowp int vLayer;
