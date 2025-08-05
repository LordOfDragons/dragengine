// OpenGL Spir-V
#ifdef WITH_SPIRV_OPENGL

#define WITH_SPECIALIZATIONS

#define HIGHP highp
#define ARG_SAMP_HIGHP
#define ARG_SAMP_MEDP
#define ARG_SAMP_LOWP
#define ARG_CONST const
#define GLSL_450
#define GS_INSTANCING
#define SUPPORTS_VSLAYER
#define SUPPORTS_VSDRAWPARAM
#define HW_DEPTH_COMPARE

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

layout(constant_id=11) const bool BlurPass2 = false;

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
layout(constant_id=39) const bool VSRenderStereo = false;
layout(constant_id=40) const bool GSRenderStereo = false;
layout(constant_id=41) const bool VSLayer = false;
layout(constant_id=42) const bool GSLayer = false;
layout(constant_id=43) const bool FullScreenQuad = false;
layout(constant_id=44) const bool FullScreenQuadSCTransform = false;
layout(constant_id=45) const bool FullScreenQuadTCTransform = false;
layout(constant_id=46) const bool NoPosTransform = false;
layout(constant_id=47) const bool NoTCTransform = false;
layout(constant_id=48) const bool TexCoordFlipY = false;

layout(constant_id=49) const int LightMode = 0;
	// 0: point
	// 1: spot
	// 2: projector
	// 3: sky
	// 4: particle

layout(constant_id=50) const bool NoiseTap = false;
layout(constant_id=51) const int PcfMode = 0;
	// 0: 1-tap
	// 1: 4-tap
	// 2: 9-tap
	// 3: variable tap
layout(constant_id=52) const bool AmbientLighting = false;
layout(constant_id=53) const bool GIRay = false;
layout(constant_id=54) const bool LuminanceOnly = false;
layout(constant_id=55) const int Shadow1Mode = 0;
	// 0: map
	// 1: cube
	// 2: dual paraboloid
	// 3: pyramid
layout(constant_id=56) const int Shadow2Mode = 0; // same as Shadow1Mode
layout(constant_id=57) const bool ShadowInverseDepth = false;
layout(constant_id=58) const bool ShaMat2EqualsShaMat1 = false;
layout(constant_id=59) const int TextureLightColor = 0;
	// 0: 2d
	// 1: cube
	// 2: equirect
layout(constant_id=60) const bool TextureShadow1Solid = false;
layout(constant_id=61) const bool TextureShadow1Transparent = false;
layout(constant_id=62) const bool TextureShadow1Ambient = false;
layout(constant_id=63) const bool TextureShadow2Solid = false;
layout(constant_id=64) const bool TextureShadow2Transparent = false;
layout(constant_id=65) const bool TextureShadow2Ambient = false;
layout(constant_id=66) const bool WithSubsurface = false;

// OpenGL
#else

#define UNIFORM_BIND(index) layout(location=index)
#define VARYING_BIND(index) layout(location=index)

#define CT_COMPUTE_IN_SIZE_XY layout(local_size_x=LOCAL_SIZE_X, local_size_y=LOCAL_SIZE_Y) in;
#define CT_COMPUTE_IN_SIZE_XYZ layout(local_size_x=LOCAL_SIZE_X, local_size_y=LOCAL_SIZE_Y, local_size_z=LOCAL_SIZE_Z) in;

#endif
