// usage:
// #include "shared/interface/skin/tessellation_evaluate.glsl"


VARYING_BIND(0) in vec4 vTESTexCoords1[]; // color, colorTintMask
VARYING_BIND(1) in vec4 vTESTexCoords2[]; // normal, reflectivity
VARYING_BIND(2) in vec4 vTESTexCoords3[]; // emissivity, refractionDistort
VARYING_BIND(3) in vec2 vTESTexCoords4[]; // ao
VARYING_BIND(4) in vec4 vTESNormalHTMask[]; // normal, HTMask
VARYING_BIND(5) in vec4 vTESTangentFadeZ[]; // tangent, fadeZ
VARYING_BIND(6) in vec3 vTESBitangent[];
VARYING_BIND(7) in vec3 vTESReflectDir[];
VARYING_BIND(8) in vec3 vTESClipCoord[];
VARYING_BIND(9) in vec3 vTESSkinClipCoord[];
VARYING_BIND(10) in vec3 vTESPosition[];
VARYING_BIND(11) flat in ivec4 vTESIndicesFlags[]; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) in vec3 vTESParticle0[]; // size, emissivity, rotation
VARYING_BIND(13) in vec4 vTESParticle1[]; // red, green, blue, transparency
VARYING_BIND(14) flat in int vTESParticleSheetCount[];

#define vTESTCColor(index) (vTESTexCoords1[index].xy)
#define vTESTCColorTintMask(index) (vTESTexCoords1[index].zw)
#define vTESTCNormal(index) (vTESTexCoords2[index].xy)
#define vTESTCReflectivity(index) (vTESTexCoords2[index].zw)
#define vTESTCEmissivity(index) (vTESTexCoords3[index].xy)
#define vTESTCRefractionDistort(index) (vTESTexCoords3[index].zw)
#define vTESTCAO(index) (vTESTexCoords4[index].xy)
#define vTESNormal(index) (vTESNormalHTMask[index].xyz)
#define vTESTangent(index) (vTESTangentFadeZ[index].xyz)
#define vTESHTMask(index) (vTESNormalHTMask[index].w)
#define vTESFadeZ(index) (vTESTangentFadeZ[index].w)
#define vTESDoubleSided(index) (vTESIndicesFlags[index].x)
#define vTESSPBIndex(index) (vTESIndicesFlags[index].y)
#define vTESSPBFlags(index) (vIndicesFlags[index].z)
#define vTESLayer(index) (vTESIndicesFlags[index].w)


#ifdef WITH_GEOMETRY_SHADER
	// opengl
	const bool PassOnNextStage = true;
	#include "shared/interface/skin/out_geometry.glsl"
	
#else
	// spir-v
	const bool PassOnNextStage = WithGeometryShader;
	#include "shared/interface/skin/out_fragment.glsl"
#endif

#include "shared/interface/skin/out_redirect.glsl"


#include "shared/tessellation.glsl"


// default outputs
void tessellationShaderDefaultOutputs(){
	TESS_VAR_LINEAR_TRI(vTexCoords1, vTESTexCoords1);
	TESS_VAR_LINEAR_TRI(vTexCoords2, vTESTexCoords2);
	TESS_VAR_LINEAR_TRI(vTexCoords3, vTESTexCoords3);
	TESS_VAR_LINEAR_TRI(vTexCoords4, vTESTexCoords4);
	TESS_VAR_LINEAR_TRI(vNormalHTMask, vTESNormalHTMask);
	TESS_VAR_LINEAR_TRI(vTangentFadeZ, vTESTangentFadeZ);
	TESS_VAR_LINEAR_TRI(vBitangent, vTESBitangent);
	TESS_VAR_LINEAR_TRI(vReflectDir, vTESReflectDir);
	TESS_VAR_LINEAR_TRI(vClipCoord, vTESClipCoord);
	TESS_VAR_LINEAR_TRI(vSkinClipCoord, vTESSkinClipCoord);
	TESS_VAR_LINEAR_TRI(vPosition, vTESPosition);
	
	vIndicesFlags = vTESIndicesFlags[0];
	
	TESS_VAR_LINEAR_TRI(vParticle0, vTESParticle0);
	TESS_VAR_LINEAR_TRI(vParticle1, vTESParticle1);
	vParticleSheetCount = vTESParticleSheetCount[0];
}
