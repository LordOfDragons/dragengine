// global constants
#include "shared/normal/constants.glsl"
#include "shared/defren/constants.glsl"
#include "shared/defren/gi/constants.glsl"
#include "shared/defren/light/constants.glsl"

// OpenGL Spir-V
#ifdef WITH_SPIRV_OPENGL

#define WITH_SPECIALIZATIONS

#define HIGHP highp
#define ARG_SAMP_HIGHP
#define ARG_SAMP_MEDP
#define ARG_SAMP_LOWP
#define GLSL_450
#define GS_INSTANCING
#define SUPPORTS_VSLAYER
#define SUPPORTS_VSDRAWPARAM
#define HW_DEPTH_COMPARE
#define SHARED_SPB
#define SHARED_SPB_USE_SSBO
#define SPB_SSBO_INSTANCE_ARRAY
#define USE_UNIFORM_CONST
#define VARCONST const

#define UNIFORM_BIND(index) layout(location=index)
#define VARYING_BIND(index) layout(location=index)

#define CT_COMPUTE_IN_SIZE_XY layout(local_size_x_id=0, local_size_y_id=1) in;
#define CT_COMPUTE_IN_SIZE_XYZ layout(local_size_x_id=0, local_size_y_id=1, local_size_z_id=2) in;

layout(constant_id=3) const bool TransformInPlace = false;

layout(constant_id=4) const bool WithRayCache = false;
layout(constant_id=5) const bool GIUseRayCache = false;
layout(constant_id=6) const bool GIRayCastDistanceOnly = false;
layout(constant_id=7) const bool GIRayCastOccMeshOnly = false;
layout(constant_id=8) const bool RenderDocDebugGI = false;
layout(constant_id=9) const int GIClearProbesCount = 64; // used as array size. has to be max possible value!
layout(constant_id=10) const bool MapIrradiance = false;

layout(constant_id=11) const int RenderPass = 0;

layout(constant_id=12) const bool DualOccMap = false;
layout(constant_id=13) const bool EnsureMinSize = false;
layout(constant_id=14) const bool FrustumTest = false;
layout(constant_id=15) const bool RenderDocDebugOccTest = false;
layout(constant_id=16) const bool WithComputeRenderTask = false;

layout(constant_id=17) const bool CullViewFrustum = false;
layout(constant_id=18) const bool CullSkyLightFrustum = false;
layout(constant_id=19) const bool CullSkyLightGIBox = false;
layout(constant_id=20) const bool CullTooSmall = false;
layout(constant_id=21) const bool WriteCullResult = false;
layout(constant_id=22) const bool WithOcclusion = false;
layout(constant_id=23) const bool ClearCullResult = false;
layout(constant_id=24) const bool WithCalcLod = false;

layout(constant_id=25) const bool WithTexture = false;
layout(constant_id=26) const bool WithRenderWorld = false;
layout(constant_id=27) const bool WithMask = false;
layout(constant_id=28) const bool InverseDepth = false;
layout(constant_id=29) const bool WithDepth = false;

layout(constant_id=30) const bool DepthDifferenceWeighting = false;
layout(constant_id=31) const bool InputArrayTextures = false;
layout(constant_id=32) const int OutDataSize = 4;
layout(constant_id=33) const int OutDataSwizzle = 0;
	// 0: rgba
	// 1: gggg
layout(constant_id=34) const int TapCount = 1;
layout(constant_id=35) const int TexDataSize = 1;
layout(constant_id=36) const int TexDataSwizzle = 0;
	// 0: rgba
	// 1: gggg
layout(constant_id=37) const bool TextureLevel = false;

layout(constant_id=38) const bool NoTexCoord = false;
layout(constant_id=39) const bool VSRenderLayer = false;
layout(constant_id=40) const int LayeredRendering = LayeredRenderingNone;
layout(constant_id=41) const bool FullScreenQuad = false;
layout(constant_id=42) const bool FullScreenQuadSCTransform = false;
layout(constant_id=43) const bool FullScreenQuadTCTransform = false;
layout(constant_id=44) const bool NoPosTransform = false;
layout(constant_id=45) const bool NoTCTransform = false;
layout(constant_id=46) const bool TexCoordFlipY = false;
layout(constant_id=47) const int LightMode = LightModePoint;
layout(constant_id=48) const bool NoiseTap = false;
layout(constant_id=49) const int PcfMode = PcfMode1Tap;
layout(constant_id=50) const bool AmbientLighting = false;
layout(constant_id=51) const bool GIRay = false;

layout(constant_id=52) const int Shadow1Mode = ShadowModeMap;
layout(constant_id=53) const int Shadow2Mode = ShadowModeMap;
layout(constant_id=54) const bool ShadowInverseDepth = false;
layout(constant_id=55) const bool ShaMat2EqualsShaMat1 = false;
layout(constant_id=56) const int TextureLightColor = TextureLightColorNone;
layout(constant_id=57) const bool TextureShadow1Solid = false;
layout(constant_id=58) const bool TextureShadow1Transparent = false;
layout(constant_id=59) const bool TextureShadow1Ambient = false;
layout(constant_id=60) const bool TextureShadow2Solid = false;
layout(constant_id=61) const bool TextureShadow2Transparent = false;
layout(constant_id=62) const bool TextureShadow2Ambient = false;
layout(constant_id=63) const bool WithSubsurface = false;
layout(constant_id=64) const int MaterialNormalDec = MaterialNormalModeFloat;
layout(constant_id=65) const int MaterialNormalEnc = MaterialNormalModeFloat;

layout(constant_id=66) const bool DepthDistance = false;
layout(constant_id=67) const int ParticleMode = ParticleModeParticle;
layout(constant_id=68) const int SSAOResolutionCount = 1;
layout(constant_id=69) const bool DepthCubeMap = false;
layout(constant_id=70) const bool DepthInput = false;
layout(constant_id=71) const bool AmbientMap = false;
layout(constant_id=72) const bool CopyColor = false;
layout(constant_id=73) const bool EncodedDepth = false;
layout(constant_id=74) const int DepthTest = DepthTestNone;
layout(constant_id=75) const bool DepthOrthogonal = false;
layout(constant_id=76) const bool WithShadowMap = false;
layout(constant_id=77) const bool DepthOffset = false;
layout(constant_id=78) const bool UseClipPlane = false;
layout(constant_id=79) const bool PerspectiveToLinear = false;
layout(constant_id=80) const int GeometryMode = GeometryModeComponent;

layout(constant_id=81) const bool TextureColor = false;
layout(constant_id=82) const bool TextureColorTintMask = false;
layout(constant_id=83) const bool TextureTransparency = false;
layout(constant_id=84) const bool TextureSolidity = false;
layout(constant_id=85) const bool TextureNormal = false;
layout(constant_id=86) const bool TextureHeight = false;
layout(constant_id=87) const bool TextureReflectivity = false;
layout(constant_id=88) const bool TextureRoughness = false;
layout(constant_id=89) const bool TextureEnvMap = false;
layout(constant_id=90) const bool TextureEnvMapEqui = false;
layout(constant_id=91) const bool TextureEnvMapFade = false;
layout(constant_id=92) const bool TextureEmissivity = false;
layout(constant_id=93) const bool TextureAbsorption = false;
layout(constant_id=94) const bool TextureRenderColor = false;
layout(constant_id=95) const bool TextureRefractionDistort = false;
layout(constant_id=96) const bool TextureAO = false;
layout(constant_id=97) const bool TextureEnvRoom = false;
layout(constant_id=98) const bool TextureEnvRoomMask = false;
layout(constant_id=99) const bool TextureEnvRoomEmissivity = false;
layout(constant_id=100) const bool TextureRimEmissivity = false;
layout(constant_id=101) const bool TextureNonPbrAlbedo = false;
layout(constant_id=102) const bool TextureNonPbrMetalness = false;

layout(constant_id=103) const bool WithVariations = false;
layout(constant_id=104) const bool MaskedSolidity = false;
layout(constant_id=105) const bool ClipPlane = false;
layout(constant_id=106) const bool EncodeOutDepth = false;
layout(constant_id=107) const bool GSRenderCubeCulling = false;
layout(constant_id=108) const int TessellationMode = TessellationModeNone;
layout(constant_id=109) const int OutputMode = OutputModeDefault;

layout(constant_id=110) const bool TPNormalStrength = false;
layout(constant_id=111) const bool TPRoughnessRemap = false;
layout(constant_id=112) const bool UseNormalRoughnessCorrection = false;
layout(constant_id=113) const bool AmbientLightProbe = false;
layout(constant_id=114) const bool SkinReflections = false;
layout(constant_id=115) const bool FadeOutRange = false;
layout(constant_id=116) const bool WithOutline = false;
layout(constant_id=117) const bool WithOutlineThicknessScreen = false;
layout(constant_id=118) const bool SkinClipPlane = false;

layout(constant_id=119) const bool DynamicColorTint = false;
layout(constant_id=120) const bool DynamicColorGamma = false;
layout(constant_id=121) const bool DynamicColorSolidityMultiplier = false;
layout(constant_id=122) const bool DynamicAOSolidityMultiplier = false;
layout(constant_id=123) const bool DynamicTransparencyMultiplier = false;
layout(constant_id=124) const bool DynamicSolidityMultiplier = false;
layout(constant_id=125) const bool DynamicHeightRemap = false;
layout(constant_id=126) const bool DynamicNormalStrength = false;
layout(constant_id=127) const bool DynamicNormalStrengthMultiplier = false;
layout(constant_id=128) const bool DynamicRoughnessRemap = false;
layout(constant_id=129) const bool DynamicRoughnessGamma = false;
layout(constant_id=130) const bool DynamicRoughnessSolidityMultiplier = false;
layout(constant_id=131) const bool DynamicReflectivitySolidityMultiplier = false;
layout(constant_id=132) const bool DynamicReflectivityMultiplier = false;
layout(constant_id=133) const bool DynamicRefractionDistortStrength = false;
layout(constant_id=134) const bool DynamicEmissivityIntensity = false;
layout(constant_id=135) const bool DynamicEnvRoomTint = false;
layout(constant_id=136) const bool DynamicEnvRoomSize = false;
layout(constant_id=137) const bool DynamicEnvRoomOffset = false;
layout(constant_id=138) const bool DynamicEnvRoomEmissivityIntensity = false;
layout(constant_id=139) const bool DynamicVariation = false;
layout(constant_id=140) const bool DynamicRimEmissivityIntensity = false;
layout(constant_id=141) const bool DynamicRimAngle = false;
layout(constant_id=142) const bool DynamicRimExponent = false;
layout(constant_id=143) const bool DynamicOutlineColor = false;
layout(constant_id=144) const bool DynamicOutlineColorTint = false;
layout(constant_id=145) const bool DynamicOutlineThickness = false;
layout(constant_id=146) const bool DynamicOutlineSolidity = false;
layout(constant_id=147) const bool DynamicOutlineEmissivity = false;
layout(constant_id=148) const bool DynamicOutlineEmissivityTint = false;
layout(constant_id=149) const bool DynamicSkinClipPlane = false;
layout(constant_id=150) const bool DynamicSkinClipPlaneBorder = false;

layout(constant_id=151) const bool WithSelector = false;
layout(constant_id=152) const bool MipMap = false;
layout(constant_id=153) const bool ClampTC = false;
layout(constant_id=154) const bool UseMinFunction = false;
layout(constant_id=155) const bool SplitLayers = false;
layout(constant_id=156) const bool WithGI = false;
layout(constant_id=157) const int EnvMapMode = EnvMapModeSingle;
layout(constant_id=158) const bool SplitVersion = false;
layout(constant_id=159) const bool SplitShiftTC = false;
layout(constant_id=160) const bool UseDepthMipMap = false;
layout(constant_id=161) const bool ResultAfterFirstLoop = false;
layout(constant_id=162) const bool RoughnessTapping = false;
layout(constant_id=163) const int SSRVersion = 0;
layout(constant_id=164) const bool Multistepping = false;
layout(constant_id=165) const bool IntegratedThresholdTest = false;
layout(constant_id=166) const bool NestedLoop = false;
layout(constant_id=167) const bool WithToneMapCurve = false;
layout(constant_id=168) const bool SampleStereo = false;
layout(constant_id=169) const bool WithReflection = false;


// OpenGL
#else

#define UNIFORM_BIND(index) layout(location=index)
#define VARYING_BIND(index) layout(location=index)

#define CT_COMPUTE_IN_SIZE_XY layout(local_size_x=LOCAL_SIZE_X, local_size_y=LOCAL_SIZE_Y) in;
#define CT_COMPUTE_IN_SIZE_XYZ layout(local_size_x=LOCAL_SIZE_X, local_size_y=LOCAL_SIZE_Y, local_size_z=LOCAL_SIZE_Z) in;

// nVidia has a problematic shader compiler which assigns and evaluates sampkers already
// in the compile phase instead of the link phase as AMD does. this causes the maximum
// sampler limit to be hit although after linking and dead code removal it would be clear
// that the limit never can be reached. setting the symbol below enables a workaround for
// this problem. it is safe to keep this enabled since it works also on the good AMD compiler
#define NVIDIA_SAMPLER_COUNT_WORKAROUND

#endif


// Shared

/*
workaround for buggy shader compilers. the sane solution is to use defines for the
possible constants and use these to make code readable:

#define LAYERED_RENDERING_STEREO 1
#define LAYERED_RENDERING_CUBE 2
#define LAYERED_RENDERING_CASCADED 3

#if LAYERED_RENDERING == LAYERED_RENDERING_CUBE
...
#endif

unfortunately with these buggy compilers an error is thrown for the LAYERED_RENDERING_CUBE.
hence the buggy compilers fails to replace the second symbol with the value 2 throwing an
error instead. the work-around is to do the testing here and to define special symbols.
this keeps the readability intact while avoiding problems with buggy compilers.

NOTE: spir-v compilers are not affected by this bug only opengl drivers it seems.
*/

#if LAYERED_RENDERING == 1
	#define LAYERED_RENDERING_STEREO
#elif LAYERED_RENDERING == 2
	#define LAYERED_RENDERING_CUBE
#elif LAYERED_RENDERING == 3
	#define LAYERED_RENDERING_CASCADED
#endif

#if defined LAYERED_RENDERING && ! defined VS_RENDER_LAYER
	#define WITH_GEOMETRY_SHADER
#endif
#if GEOMETRY_MODE == 5 && ! defined WITH_GEOMETRY_SHADER
	#define WITH_GEOMETRY_SHADER
#endif
#if LIGHT_MODE == 4 && ! defined WITH_GEOMETRY_SHADER
	#define WITH_GEOMETRY_SHADER
#endif

const bool WithGeometryShader = (LayeredRendering != LayeredRenderingNone && !VSRenderLayer)
	|| GeometryMode == GeometryModeParticle;


// #define TESSELLATION_MODE_LINEAR 1
#if TESSELLATION_MODE == 1
	#define TESSELLATION_MODE_LINEAR
#endif

#ifdef TESSELLATION_MODE
	#define WITH_TESSELLATION_SHADER
#endif

const bool WithTessellationShader = TessellationMode != TessellationModeNone;


const bool AnyKindBillboard = GeometryMode == GeometryModeBillboard
	|| GeometryMode == GeometryModePropFieldImposter;

const bool AnyKindPropField = GeometryMode == GeometryModePropField
	|| GeometryMode == GeometryModePropFieldImposter;

const bool AnyOutputModeMaterial = OutputMode == OutputModeMaterial
	|| OutputMode == OutputModeMaterialReduced;
