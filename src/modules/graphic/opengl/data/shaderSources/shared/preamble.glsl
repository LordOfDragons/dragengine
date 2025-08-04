// OpenGL Spir-V
#ifdef WITH_SPIRV_OPENGL

#define WITH_SPECIALIZATIONS 1

#define HIGHP highp
#define ARG_SAMP_HIGHP
#define ARG_SAMP_MEDP
#define ARG_SAMP_LOWP

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
layout(constant_id=12) const bool DecodeInDepth = false;

layout(constant_id=13) const bool DualOccMap = false;
layout(constant_id=14) const bool EnsureMinSize = false;
layout(constant_id=15) const bool FrustumTest = false;
layout(constant_id=16) const bool RenderDocDebugOccTest = false;
layout(constant_id=17) const bool WithComputeRenderTask = false;

layout(constant_id=18) const bool CullViewFrustum = false;
layout(constant_id=19) const bool CullSkyLightFrustum = false;
layout(constant_id=20) const bool CullSkyLightGIBox = false;
layout(constant_id=21) const bool CullTooSmall = false;
layout(constant_id=22) const bool WriteCullResult = false;
layout(constant_id=23) const bool WithOcclusion = false;
layout(constant_id=24) const bool ClearCullResult = false;
layout(constant_id=25) const bool WithCalcLod = false;

layout(constant_id=26) const bool WithTexture = false;
layout(constant_id=27) const bool WithRenderWorld = false;
layout(constant_id=28) const bool WithMask = false;
layout(constant_id=29) const bool InverseDepth = false;
layout(constant_id=30) const bool WithDepth = false;

layout(constant_id=31) const bool DepthDifferenceWeighting = false;
layout(constant_id=32) const bool InputArrayTextures = false;
layout(constant_id=33) const int OutDataSize = 4;
layout(constant_id=34) const int OutDataSwizzle = 0;
	// 0: rgba
	// 1: gggg
layout(constant_id=35) const int TapCount = 1;
layout(constant_id=36) const int TexDataSize = 1;
layout(constant_id=37) const int TexDataSwizzle = 0;
	// 0: rgba
	// 1: gggg
layout(constant_id=38) const bool TextureLevel = false;

// OpenGL
#else

#define UNIFORM_BIND(index) layout(location=index)
#define VARYING_BIND(index) layout(location=index)

#define CT_COMPUTE_IN_SIZE_XY layout(local_size_x=LOCAL_SIZE_X, local_size_y=LOCAL_SIZE_Y) in;
#define CT_COMPUTE_IN_SIZE_XYZ layout(local_size_x=LOCAL_SIZE_X, local_size_y=LOCAL_SIZE_Y, local_size_z=LOCAL_SIZE_Z) in;

#endif
