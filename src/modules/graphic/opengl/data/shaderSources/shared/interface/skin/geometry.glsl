// usage:
// #include "shared/interface/skin/geometry.glsl"


// configuration
#ifndef GEOMETRY_SHADER_INPUT_SIZE
	#define GEOMETRY_SHADER_INPUT_SIZE 3
#endif


VARYING_BIND(0) in mediump vec4 vGSTexCoords1[GEOMETRY_SHADER_INPUT_SIZE]; // color, colorTintMask
VARYING_BIND(1) in mediump vec4 vGSTexCoords2[GEOMETRY_SHADER_INPUT_SIZE]; // normal, reflectivity
VARYING_BIND(2) in mediump vec4 vGSTexCoords3[GEOMETRY_SHADER_INPUT_SIZE]; // emissivity, refractionDistort
VARYING_BIND(3) in lowp vec2 vGSTexCoords4[GEOMETRY_SHADER_INPUT_SIZE]; // ao
VARYING_BIND(4) in mediump vec4 vGSNormalHTMask[GEOMETRY_SHADER_INPUT_SIZE]; // normal, HTMask
VARYING_BIND(5) in mediump vec4 vGSTangentFadeZ[GEOMETRY_SHADER_INPUT_SIZE]; // tangent, fadeZ
VARYING_BIND(6) in mediump vec3 vGSBitangent[GEOMETRY_SHADER_INPUT_SIZE];
VARYING_BIND(7) in mediump vec3 vGSReflectDir[GEOMETRY_SHADER_INPUT_SIZE];
VARYING_BIND(8) in mediump vec3 vGSClipCoord[GEOMETRY_SHADER_INPUT_SIZE];
VARYING_BIND(9) in mediump vec3 vGSSkinClipCoord[GEOMETRY_SHADER_INPUT_SIZE];
VARYING_BIND(10) in mediump vec3 vGSPosition[GEOMETRY_SHADER_INPUT_SIZE];
VARYING_BIND(11) flat in mediump ivec4 vGSIndicesFlags[GEOMETRY_SHADER_INPUT_SIZE]; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) in mediump vec3 vGSParticle0[GEOMETRY_SHADER_INPUT_SIZE]; // size, emissivity, rotation
VARYING_BIND(13) in lowp vec4 vGSParticle1[GEOMETRY_SHADER_INPUT_SIZE]; // red, green, blue, transparency
VARYING_BIND(14) flat in lowp int vGSParticleSheetCount[GEOMETRY_SHADER_INPUT_SIZE];

#define vGSTCColor(index) (vGSTexCoords1[index].xy)
#define vGSTCColorTintMask(index) (vGSTexCoords1[index].zw)
#define vGSTCNormal(index) (vGSTexCoords2[index].xy)
#define vGSTCReflectivity(index) (vGSTexCoords2[index].zw)
#define vGSTCEmissivity(index) (vGSTexCoords3[index].xy)
#define vGSTCRefractionDistort(index) (vGSTexCoords3[index].zw)
#define vGSTCAO(index) (vGSTexCoords4[index].xy)
#define vGSNormal(index) (vGSNormalHTMask[index].xyz)
#define vGSTangent(index) (vGSTangentFadeZ[index].xyz)
#define vGSHTMask(index) (vGSNormalHTMask[index].w)
#define vGSFadeZ(index) (vGSTangentFadeZ[index].w)
#define vGSDoubleSided(index) (vGSIndicesFlags[index].x)
#define vGSSPBIndex(index) (vGSIndicesFlags[index].y)
#define vGSSPBFlags(index) (vGSIndicesFlags[index].z)
#define vGSLayer(index) (vGSIndicesFlags[index].w)


#include "shared/interface/skin/out_fragment.glsl"

#include "shared/interface/skin/out_redirect.glsl"


// default outputs
void geometryShaderDefaultOutputs(int index, int layer){
	vTexCoords1 = vGSTexCoords1[index];
	vTexCoords2 = vGSTexCoords2[index];
	vTexCoords3 = vGSTexCoords3[index];
	vTexCoords4 = vGSTexCoords4[index];
	vNormalHTMask = vGSNormalHTMask[index];
	vTangentFadeZ = vGSTangentFadeZ[index];
	vBitangent = vGSBitangent[index];
	vReflectDir = vGSReflectDir[index];
	vClipCoord = vGSClipCoord[index];
	vSkinClipCoord = vGSSkinClipCoord[index];
	vPosition = vGSPosition[index];
	vIndicesFlags = vGSIndicesFlags[index];
	
	vParticle0 = vGSParticle0[index];
	vParticle1 = vGSParticle1[index];
	vParticleSheetCount = vGSParticleSheetCount[index];
	
	vLayer = layer;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
}
