// usage:
// #include "shared/interface/skin/samplers.glsl"

layout(binding=0) uniform lowp sampler2D texColor;
layout(binding=0) uniform lowp sampler2DArray texColorArray;

layout(binding=1) uniform lowp sampler2D texTransparency;
layout(binding=1) uniform lowp sampler2DArray texTransparencyArray;

layout(binding=2) uniform lowp sampler2D texColorTintMask;
layout(binding=2) uniform lowp sampler2DArray texColorTintMaskArray;

layout(binding=3) uniform lowp sampler2D texSolidity;
layout(binding=3) uniform lowp sampler2DArray texSolidityArray;

layout(binding=4) uniform lowp sampler2D texNormal;
layout(binding=4) uniform lowp sampler2DArray texNormalArray;

layout(binding=5) uniform lowp sampler2D texHeight;
layout(binding=5) uniform lowp sampler2DArray texHeightArray;

layout(binding=6) uniform lowp sampler2D texReflectivity;
layout(binding=6) uniform lowp sampler2DArray texReflectivityArray;

layout(binding=7) uniform lowp sampler2D texRoughness;
layout(binding=7) uniform lowp sampler2DArray texRoughnessArray;

// TextureEnvMapEqui
layout(binding=8) uniform mediump sampler2D texEnvMap;
layout(binding=9) uniform mediump sampler2D texEnvMapFade;

// !TextureEnvMapEqui
layout(binding=8) uniform mediump samplerCube texEnvMapCube;
layout(binding=9) uniform mediump samplerCube texEnvMapFadeCube;

layout(binding=10) uniform mediump sampler2D texEmissivity;
layout(binding=10) uniform mediump sampler2DArray texEmissivityArray;

layout(binding=11) uniform mediump sampler2DArray texRenderColor;

layout(binding=12) uniform lowp sampler2D texRefractionDistort;
layout(binding=12) uniform lowp sampler2DArray texRefractionDistortArray;

layout(binding=13) uniform lowp sampler2D texAO;
layout(binding=13) uniform lowp sampler2DArray texAOArray;

layout(binding=14) uniform lowp samplerCube texEnvRoom;

layout(binding=15) uniform lowp sampler2D texEnvRoomMask;
layout(binding=15) uniform lowp sampler2DArray texEnvRoomMaskArray;

layout(binding=16) uniform mediump samplerCube texEnvRoomEmissivity;

layout(binding=17) uniform lowp sampler2D texAbsorption;
layout(binding=17) uniform lowp sampler2DArray texAbsorptionArray;

layout(binding=18) uniform mediump sampler2D texRimEmissivity;
layout(binding=18) uniform mediump sampler2DArray texRimEmissivityArray;

layout(binding=0) uniform lowp sampler2D texNonPbrAlbedo;
layout(binding=0) uniform lowp sampler2DArray texNonPbrAlbedoArray;

layout(binding=6) uniform lowp sampler2D texNonPbrMetalness;
layout(binding=6) uniform lowp sampler2DArray texNonPbrMetalnessArray;

layout(binding=19) uniform HIGHP sampler2DArray texDepthTest;
