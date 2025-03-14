// R16F/RG16F image format handling
/////////////////////////////////////
//
// Use IMG_R16F_FMT / IMG_RG16F_FMT as layout format like this:
//   layout(binding=0, IMG_R16F_FMT) uniform writeonly restrict mediump IMG_R16F_2DARR texImage;
//
// Use IMG_R16F_STORE / IMG_RG16F_STORE to write value to image like this:
//   imageStore(texImage, tc, IMG_R16F_STORE(value));
//   imageStore(texImage, tc, IMG_RG16F_STORE(value));
//
// Use IMG_R16F_LOAD / IMG_RG16F_LOAD to read value from image like this:
//   IMG_R16F_LOAD(imageLoad(texImage, tc));
//   IMG_RG16F_LOAD(imageLoad(texImage, tc)).rg;
//
// Unfortunately not all can handle r16ui nor r32ui. We are thus back to wasting
// tons of memory by using rgba16f since only this one works

#ifdef BROKEN_R16F_RG16F_IMAGE_FORMAT
	#define IMG_R16F_FMT r16ui
	#define IMG_RG16F_FMT r32ui
	
	#define IMG_R16F_2DARR uimage2DArray
	#define IMG_RG16F_2DARR uimage2DArray
	
	#define IMG_R16F_2D image2D
	#define IMG_RG16F_2D image2D
	
	// packHalf2x16 stores input float in 32-bit uint like this:
	// - v.r => 0x0000ffff
	// - v.g => 0xffff0000
	//
	// hence for R16F the value is stored in the red component
	// since the green component will be lost
	#define IMG_R16F_STORE(v) uvec4(packHalf2x16(vec2((float(v), 0)))
	#define IMG_R16F_LOAD(v) float(unpackHalf2x16(uint(v)))
	
	#define IMG_RG16F_STORE(v) uvec4(packHalf2x16(vec2(v)))
	#define IMG_RG16F_LOAD(v) vec2(unpackHalf2x16(uint(v)))
	
#else
	#define IMG_R16F_FMT r16f
	#define IMG_RG16F_FMT rg16f
	
	#define IMG_R16F_2DARR image2DArray
	#define IMG_RG16F_2DARR image2DArray
	
	#define IMG_R16F_2D image2D
	#define IMG_RG16F_2D image2D
	
	#define IMG_R16F_STORE(v) vec4(float(v))
	#define IMG_R16F_LOAD(v) float(v)
	
	#define IMG_RG16F_STORE(v) vec4(vec2(v), 0, 0)
	#define IMG_RG16F_LOAD(v) vec2(v)
#endif
