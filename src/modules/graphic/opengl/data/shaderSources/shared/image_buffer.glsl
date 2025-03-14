// Image Format Handling
//////////////////////////
//
// OpenGL ES supports only very limited set of image buffer formats compared to OpenGL.
// These definitions help to deal with the problem. Here the list of supported formats:
// - rgba32f
// - rgba16f
// - r32f
// - rgba8
// - rgba8_snorm
// - rgba32i
// - rgba16i
// - rgba8i
// - r32i
// - rgba32ui
// - rgba16ui
// - rgba8ui
// - r32ui
//
// Use IMG_*_FMT as layout format like this:
//   layout(binding=0, IMG_R16F_FMT) uniform writeonly restrict mediump IMG_R16F_2DARR texImage;
//
// Use IMG_*_STORE to write value to image like this:
//   imageStore(texImage, tc, IMG_R16F_STORE(value));
//
// Use IMG_*_LOAD to read value from image like this:
//   IMG_R16F_LOAD(imageLoad(texImage, tc));

#ifdef RESTRICTED_IMAGE_BUFFER_FORMATS
	#define IMG_R16F_FMT rgba16f
	#define IMG_RG16F_FMT rgba16f
	#define IMG_R8_FMT rgba8
	
#else
	#define IMG_R16F_FMT r16f
	#define IMG_RG16F_FMT rg16f
	#define IMG_R8_FMT r8
#endif

#define IMG_R16F_2DARR image2DArray
#define IMG_RG16F_2DARR image2DArray
#define IMG_R8_2DARR image2DArray

#define IMG_R16F_2D image2D
#define IMG_RG16F_2D image2D
#define IMG_R8_2D image2D

#define IMG_R16F_STORE(v) vec4(float(v))
#define IMG_R16F_LOAD(v) float(v)

#define IMG_R8_STORE(v) vec4(float(v))
#define IMG_R8_LOAD(v) float(v)

#define IMG_RG16F_STORE(v) vec4(vec2(v), 0, 0)
#define IMG_RG16F_LOAD(v) vec2(v)
