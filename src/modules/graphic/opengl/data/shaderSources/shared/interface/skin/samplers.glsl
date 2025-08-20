// usage:
// #include "shared/interface/skin/samplers.glsl"

#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
	#ifdef WITH_VARIATION
		#define DTNA(e)
		#define DTA(e) e;
	#else
		#define DTNA(e) e;
		#define DTA(e)
	#endif
	
	#ifdef TEXTURE_ENVMAP_EQUI
		#define DTE(e) e;
		#define DTNE(e)
		#define SAMPLE_ENVMAP(s,tc,l) vec4(0.0)
		#define SAMPLE_ENVMAP_EQUI(s,tc,l) textureLod(s, tc, l)
	#else
		#define DTE(e)
		#define DTNE(e) e;
		#define SAMPLE_ENVMAP(s,tc,l) textureLod(s, tc, l)
		#define SAMPLE_ENVMAP_EQUI(s,tc,l) vec4(0.0)
	#endif
	
#else
	#define DTNA(e) e;
	#define DTA(e) e;
	#define DTE(e) e;
	#define DTNE(e) e;
	#define SAMPLE_ENVMAP(s,tc,l) textureLod(s, tc, l)
	#define SAMPLE_ENVMAP_EQUI(s,tc,l) textureLod(s, tc, l)
#endif



DTNA(layout(binding=0) uniform lowp sampler2D texColor)
DTA(layout(binding=0) uniform lowp sampler2DArray texColorArray)

DTNA(layout(binding=1) uniform lowp sampler2D texTransparency)
DTA(layout(binding=1) uniform lowp sampler2DArray texTransparencyArray)

DTNA(layout(binding=2) uniform lowp sampler2D texColorTintMask)
DTA(layout(binding=2) uniform lowp sampler2DArray texColorTintMaskArray)

DTNA(layout(binding=3) uniform lowp sampler2D texSolidity)
DTA(layout(binding=3) uniform lowp sampler2DArray texSolidityArray)

DTNA(layout(binding=4) uniform lowp sampler2D texNormal)
DTA(layout(binding=4) uniform lowp sampler2DArray texNormalArray)

DTNA(layout(binding=5) uniform lowp sampler2D texHeight)
DTA(layout(binding=5) uniform lowp sampler2DArray texHeightArray)

DTNA(layout(binding=6) uniform lowp sampler2D texReflectivity)
DTA(layout(binding=6) uniform lowp sampler2DArray texReflectivityArray)

DTNA(layout(binding=7) uniform lowp sampler2D texRoughness)
DTA(layout(binding=7) uniform lowp sampler2DArray texRoughnessArray)

// TextureEnvMapEqui
DTE(layout(binding=8) uniform mediump sampler2D texEnvMap)
DTE(layout(binding=9) uniform mediump sampler2D texEnvMapFade)

// !TextureEnvMapEqui
DTNE(layout(binding=8) uniform mediump samplerCube texEnvMapCube)
DTNE(layout(binding=9) uniform mediump samplerCube texEnvMapFadeCube)

DTNA(layout(binding=10) uniform mediump sampler2D texEmissivity)
DTA(layout(binding=10) uniform mediump sampler2DArray texEmissivityArray)

layout(binding=11) uniform mediump sampler2DArray texRenderColor;

DTNA(layout(binding=12) uniform lowp sampler2D texRefractionDistort)
DTA(layout(binding=12) uniform lowp sampler2DArray texRefractionDistortArray)

DTNA(layout(binding=13) uniform lowp sampler2D texAO)
DTA(layout(binding=13) uniform lowp sampler2DArray texAOArray)

layout(binding=14) uniform lowp samplerCube texEnvRoom;

DTNA(layout(binding=15) uniform lowp sampler2D texEnvRoomMask)
DTA(layout(binding=15) uniform lowp sampler2DArray texEnvRoomMaskArray)

layout(binding=16) uniform mediump samplerCube texEnvRoomEmissivity;

DTNA(layout(binding=17) uniform lowp sampler2D texAbsorption)
DTA(layout(binding=17) uniform lowp sampler2DArray texAbsorptionArray)

DTNA(layout(binding=18) uniform mediump sampler2D texRimEmissivity)
DTA(layout(binding=18) uniform mediump sampler2DArray texRimEmissivityArray)

//DTNA(layout(binding=0) uniform lowp sampler2D texNonPbrAlbedo)
//DTA(layout(binding=0) uniform lowp sampler2DArray texNonPbrAlbedoArray)
#define texNonPbrAlbedo texColor
#define texNonPbrAlbedoArray texColorArray

//DTNA(layout(binding=6) uniform lowp sampler2D texNonPbrMetalness)
//DTA(layout(binding=6) uniform lowp sampler2DArray texNonPbrMetalnessArray)
#define texNonPbrMetalness texReflectivity
#define texNonPbrMetalnessArray texReflectivityArray

layout(binding=19) uniform HIGHP sampler2DArray texDepthTest;



#undef DTNA
#undef DTA
#undef DTE
#undef DTNE
