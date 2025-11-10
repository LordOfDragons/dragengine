// usage:
// #include "shared/interface/skin/fragment.glsl"

VARYING_BIND(0) in mediump vec4 vTexCoords1; // color, colorTintMask
VARYING_BIND(1) in mediump vec4 vTexCoords2; // normal, reflectivity
VARYING_BIND(2) in mediump vec4 vTexCoords3; // emissivity, refractionDistort
VARYING_BIND(3) in lowp vec2 vTexCoords4; // ao
VARYING_BIND(4) in mediump vec4 vNormalHTMask; // normal, HTMask
VARYING_BIND(5) in mediump vec4 vTangentFadeZ; // tangent, fadeZ
VARYING_BIND(6) in mediump vec3 vBitangent;
VARYING_BIND(7) in mediump vec3 vReflectDir;
VARYING_BIND(8) in mediump vec3 vClipCoord;
VARYING_BIND(9) in mediump vec3 vSkinClipCoord;
VARYING_BIND(10) in mediump vec3 vPosition;
VARYING_BIND(11) flat in mediump ivec4 vIndicesFlags; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) in mediump vec3 vParticle0; // size, emissivity, rotation
VARYING_BIND(13) in lowp vec4 vParticle1; // red, green, blue, transparency
VARYING_BIND(14) flat in lowp int vParticleSheetCount;

#define vTCColor (vTexCoords1.xy)
#define vTCColorTintMask (vTexCoords1.zw)
#define vTCNormal (vTexCoords2.xy)
#define vTCReflectivity (vTexCoords2.zw)
#define vTCEmissivity (vTexCoords3.xy)
#define vTCRefractionDistort (vTexCoords3.zw)
#define vTCAO (vTexCoords4.xy)
#define vNormal (vNormalHTMask.xyz)
#define vTangent (vTangentFadeZ.xyz)
#define vHTMask (vNormalHTMask.w)
#define vFadeZ (vTangentFadeZ.w)
#define vDoubleSided (vIndicesFlags.x)
#define vSPBIndex (vIndicesFlags.y)
#define vSPBFlags (vIndicesFlags.z)
#define vLayer (vIndicesFlags.w)

#define vParticleColor vParticle1
#define vParticleEmissivity (vParticle0.y)
