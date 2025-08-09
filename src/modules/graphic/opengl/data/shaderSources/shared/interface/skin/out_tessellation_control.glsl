VARYING_BIND(0) out vec4 vTCSTexCoords1; // color, colorTintMask
VARYING_BIND(1) out vec4 vTCSTexCoords2; // normal, reflectivity
VARYING_BIND(2) out vec4 vTCSTexCoords3; // emissivity, refractionDistort
VARYING_BIND(3) out vec2 vTCSTexCoords4; // ao
VARYING_BIND(4) out vec4 vTCSNormalHTMask; // normal, HTMask
VARYING_BIND(5) out vec4 vTCSTangentFadeZ; // tangent, fadeZ
VARYING_BIND(6) out vec3 vTCSBitangent;
VARYING_BIND(7) out vec3 vTCSReflectDir;
VARYING_BIND(8) out vec3 vTCSClipCoord;
VARYING_BIND(9) out vec3 vTCSSkinClipCoord;
VARYING_BIND(10) out vec3 vTCSPosition;
VARYING_BIND(11) flat out ivec4 vTCSIndicesFlags; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) out vec3 vTCSParticle0; // size, emissivity, rotation
VARYING_BIND(13) out vec4 vTCSParticle1; // red, green, blue, transparency
VARYING_BIND(14) flat out int vTCSParticleSheetCount;

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
