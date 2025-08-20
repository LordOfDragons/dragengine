VARYING_BIND(0) out vec4 vTexCoords1; // color, colorTintMask
VARYING_BIND(1) out vec4 vTexCoords2; // normal, reflectivity
VARYING_BIND(2) out vec4 vTexCoords3; // emissivity, refractionDistort
VARYING_BIND(3) out vec2 vTexCoords4; // ao
VARYING_BIND(4) out vec4 vNormalHTMask; // normal, HTMask
VARYING_BIND(5) out vec4 vTangentFadeZ; // tangent, fadeZ
VARYING_BIND(6) out vec3 vBitangent;
VARYING_BIND(7) out vec3 vReflectDir;
VARYING_BIND(8) out vec3 vClipCoord;
VARYING_BIND(9) out vec3 vSkinClipCoord;
VARYING_BIND(10) out vec3 vPosition;
VARYING_BIND(11) flat out ivec4 vIndicesFlags; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) out vec3 vParticle0; // size, emissivity, rotation
VARYING_BIND(13) out vec4 vParticle1; // red, green, blue, transparency
VARYING_BIND(14) flat out int vParticleSheetCount;
