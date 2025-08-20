VARYING_BIND(0) out vec4 vGSTexCoords1; // color, colorTintMask
VARYING_BIND(1) out vec4 vGSTexCoords2; // normal, reflectivity
VARYING_BIND(2) out vec4 vGSTexCoords3; // emissivity, refractionDistort
VARYING_BIND(3) out vec2 vGSTexCoords4; // ao
VARYING_BIND(4) out vec4 vGSNormalHTMask; // normal, HTMask
VARYING_BIND(5) out vec4 vGSTangentFadeZ; // tangent, fadeZ
VARYING_BIND(6) out vec3 vGSBitangent;
VARYING_BIND(7) out vec3 vGSReflectDir;
VARYING_BIND(8) out vec3 vGSClipCoord;
VARYING_BIND(9) out vec3 vGSSkinClipCoord;
VARYING_BIND(10) out vec3 vGSPosition;
VARYING_BIND(11) flat out ivec4 vGSIndicesFlags; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) out vec3 vGSParticle0; // size, emissivity, rotation
VARYING_BIND(13) out vec4 vGSParticle1; // red, green, blue, transparency
VARYING_BIND(14) flat out int vGSParticleSheetCount;

#define vTexCoords1 vGSTexCoords1
#define vTexCoords2 vGSTexCoords2
#define vTexCoords3 vGSTexCoords3
#define vTexCoords4 vGSTexCoords4
#define vNormalHTMask vGSNormalHTMask
#define vTangentFadeZ vGSTangentFadeZ
#define vBitangent vGSBitangent
#define vReflectDir vGSReflectDir
#define vClipCoord vGSClipCoord
#define vSkinClipCoord vGSSkinClipCoord
#define vPosition vGSPosition
#define vIndicesFlags vGSIndicesFlags

#define vParticle0 vGSParticle0
#define vParticle1 vGSParticle1
#define vParticleSheetCount vGSParticleSheetCount
