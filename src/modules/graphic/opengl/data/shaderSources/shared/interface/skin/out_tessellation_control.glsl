VARYING_BIND(0) out mediump vec4 vTCSTexCoords1; // color, colorTintMask
VARYING_BIND(1) out mediump vec4 vTCSTexCoords2; // normal, reflectivity
VARYING_BIND(2) out mediump vec4 vTCSTexCoords3; // emissivity, refractionDistort
VARYING_BIND(3) out lowp vec2 vTCSTexCoords4; // ao
VARYING_BIND(4) out mediump vec4 vTCSNormalHTMask; // normal, HTMask
VARYING_BIND(5) out mediump vec4 vTCSTangentFadeZ; // tangent, fadeZ
VARYING_BIND(6) out mediump vec3 vTCSBitangent;
VARYING_BIND(7) out mediump vec3 vTCSReflectDir;
VARYING_BIND(8) out mediump vec3 vTCSClipCoord;
VARYING_BIND(9) out mediump vec3 vTCSSkinClipCoord;
VARYING_BIND(10) out mediump vec3 vTCSPosition;
VARYING_BIND(11) flat out mediump ivec4 vTCSIndicesFlags; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) out mediump vec3 vTCSParticle0; // size, emissivity, rotation
VARYING_BIND(13) out lowp vec4 vTCSParticle1; // red, green, blue, transparency
VARYING_BIND(14) flat out lowp int vTCSParticleSheetCount;

#define vTexCoords1 vTCSTexCoords1
#define vTexCoords2 vTCSTexCoords2
#define vTexCoords3 vTCSTexCoords3
#define vTexCoords4 vTCSTexCoords4
#define vNormalHTMask vTCSNormalHTMask
#define vTangentFadeZ vTCSTangentFadeZ
#define vBitangent vTCSBitangent
#define vReflectDir vTCSReflectDir
#define vClipCoord vTCSClipCoord
#define vSkinClipCoord vTCSSkinClipCoord
#define vPosition vTCSPosition
#define vIndicesFlags vTCSIndicesFlags

#define vParticle0 vTCSParticle0
#define vParticle1 vTCSParticle1
#define vParticleSheetCount vTCSParticleSheetCount
