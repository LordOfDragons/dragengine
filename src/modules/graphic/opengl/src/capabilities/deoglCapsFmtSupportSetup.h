/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


// Texture Format information
////////////////////////////////

enum eTestTextureFormats{
	// integer formats
	ettfR8, // GL_R8
	ettfR16, // GL_R16
	ettfR8_S, // GL_R8_SNORM
	ettfR16_S, // GL_R16_SNORM
	
	ettfRG8, // GL_RG8
	ettfRG16, // GL_RG16
	ettfRG8_S, // GL_RG8_SNORM
	ettfRG16_S, // GL_RG16_SNORM
	
	ettfR3_G3_B2, // GL_R3_G3_B2
	ettfRGB4, // GL_RGB4
	ettfRGB5, // GL_RGB5
	ettfRGB8, // GL_RGB8
	ettfRGB16, // GL_RGB16
	ettfRGB8_S, // GL_RGB8_SNORM
	ettfRGB16_S, // GL_RGB16_SNORM
	
	ettfRGBA2, // GL_RGBA2
	ettfRGBA4, // GL_RGBA4
	ettfRGB5_A1, // GL_RGB5_A1
	ettfRGBA8, // GL_RGBA8
	ettfRGB10_A2, // GL_RGB10_A2
	ettfRGBA16, // GL_RGBA16
	ettfRGBA8_S, // GL_RGBA8_SNORM
	ettfRGBA16_S, // GL_RGBA16_SNORM
	
	// floating point formats
	ettfR16F, // GL_R16F
	ettfRG16F, // GL_RG16F
	ettfRGB16F, // GL_RGB16F
	ettfRGBA16F, // GL_RGBA16F
	
	ettfR32F, // GL_R32F
	ettfRG32F, // GL_RG32F
	ettfRGB32F, // GL_RGB32F
	ettfRGBA32F, // GL_RGBA32F
	
	ettfAti_RGB_F16, // GL_RGB_FLOAT16_ATI
	ettfAti_RGBA_F16, // GL_RGBA_FLOAT16_ATI
	
	ettfAti_RGB_F32, // GL_RGB_FLOAT32_ATI
	ettfAti_RGBA_F32, // GL_RGBA_FLOAT32_ATI
	
	ettfNV_R_F, // GL_FLOAT_R_NV
	ettfNV_RG_F, // GL_FLOAT_RG_NV
	ettfNV_RGB_F, // GL_FLOAT_RGB_NV
	ettfNV_RGBA_F, // GL_FLOAT_RGBA_NV
	
	ettfNV_R_F16, // GL_FLOAT_R16_NV
	ettfNV_RG_F16, // GL_FLOAT_RG16_NV
	ettfNV_RGB_F16, // GL_FLOAT_RGB16_NV
	ettfNV_RGBA_F16, // GL_FLOAT_RGBA16_NV
	
	ettfNV_R_F32, // GL_FLOAT_R32_NV
	ettfNV_RG_F32, // GL_FLOAT_RG32_NV
	ettfNV_RGB_F32, // GL_FLOAT_RGB32_NV
	ettfNV_RGBA_F32, // GL_FLOAT_RGBA32_NV
	
	ettfRG11B10F, // GL_R11F_G11F_B10F
	
	// integral formats
	ettfR8I, // GL_R8I
	ettfR8UI, // GL_R8UI
	ettfR16I, // GL_R16I
	ettfR16UI, // GL_R16UI
	
	ettfRG8I, // GL_RG8I
	ettfRG8UI, // GL_RG8UI
	ettfRG16I, // GL_RG16I
	ettfRG16UI, // GL_RG16UI
	
	ettfRGB8I, // GL_RGB8I
	ettfRGB8UI, // GL_RGB8UI
	ettfRGB16I, // GL_RGB16I
	ettfRGB16UI, // GL_RGB16UI
	
	ettfRGBA8I, // GL_RGBA8I
	ettfRGBA8UI, // GL_RGBA8UI
	ettfRGBA16I, // GL_RGBA16I
	ettfRGBA16UI, // GL_RGBA16UI
	
	// compressed integer formats
	ettfCompress_DXT1_RGB, // GL_COMPRESSED_RGB_S3TC_DXT1
	ettfCompress_DXT1_RGBA, // GL_COMPRESSED_RGBA_S3TC_DXT1
	ettfCompress_DXT3_RGBA, // GL_COMPRESSED_RGBA_S3TC_DXT3
	ettfCompress_DXT5_RGBA, // GL_COMPRESSED_RGBA_S3TC_DXT5
	
	ettfCompress_FXT1_RGB, // GL_COMPRESSED_RGB_FXT1_3DFX
	ettfCompress_FXT1_RGBA, // GL_COMPRESSED_RGBA_FXT1_3DFX
	
	ettfCompress_S3TC_RGB, // GL_RGB_S3TC
	ettfCompress_S3TC_RGB4, // GL_RGB4_S3TC
	ettfCompress_S3TC_RGBA, // GL_RGBA_S3TC
	ettfCompress_S3TC_RGBA4, // GL_RGBA4_S3TC
	
	ettfCompress_R, // GL_COMPRESSED_RED
	ettfCompress_RG, // GL_COMPRESSED_RG
	ettfCompress_RGB, // GL_COMPRESSED_RGB
	ettfCompress_RGBA, // GL_COMPRESSED_RGBA
	
	ettfCompress_Lum_LATC1, // GL_COMPRESSED_LUMINANCE_LATC1
	ettfCompress_Sig_Lum_LATC1, // GL_COMPRESSED_SIGNED_LUMINANCE_LATC1
	ettfCompress_Lum_Alpha_LATC2, // GL_COMPRESSED_LUMINANCE_ALPHA_LATC2
	ettfCompress_Sig_Lum_Alpha_LATC2, // GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2
	
	ettfCompress_Red_RGTC1, // GL_COMPRESSED_RED_RGTC1
	ettfCompress_Sig_Red_RGTC1, // GL_COMPRESSED_SIGNED_RED_RGTC1
	ettfCompress_Red_Green_RGTC2, // GL_COMPRESSED_RED_GREEN_RGTC2
	ettfCompress_Sig_Red_Green_RGTC2, // GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2
	
	/*
	OpenGL ES information (https://arm-software.github.io/opengl-es-sdk-for-android/etc_texture.html):
	GL_COMPRESSED_R11_EAC:
		11 bits for a single channel. Useful for single channel data where higher
		than 8 bit precision is needed. For example, heightmaps.
	
	GL_COMPRESSED_SIGNED_R11_EAC:
		Signed version of GL_COMPRESSED_SIGNED_R11_EAC, useful when signed data is needed.
		
	GL_COMPRESSED_RG11_EAC:
		11 bits for two channels. Useful for two channel data where higher than 8 bit
		precision is needed. For example, normalised bump maps, the third component
		can be reconstructed from the other two components.
		
	GL_COMPRESSED_SIGNED_RG11_EAC:
		Signed version of GL_COMPRESSED_RG11_EAC, useful when signed data is needed.
		
	GL_COMPRESSED_RGB8_ETC2:
		8 bits for three channels. Useful for normal textures without alpha values.
		
	GL_COMPRESSED_SRGB8_ETC2:
		sRGB version of GL_COMPRESSED_RGB8_ETC2.
		
	GL_COMPRESSED_RGBA8_ETC2_EAC:
		8 bits for four channels. Useful for normal textures with varying alpha values.
		
	GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
		sRGB version of GL_COMPRESSED_RGBA8_ETC2_EAC.
		
	GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
		8 bits for three channels and a 1 bit alpha channel. Useful for normal textures
		with binary alpha values.
		
	GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
		sRGB version of GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2.
	*/
	
	ettfCompress_ETC2_RGB, // GL_COMPRESSED_RGB8_ETC2
	ettfCompress_ETC2_RGBA, // GL_COMPRESSED_RGBA8_ETC2_EAC
	
	// depth formats
	ettfDepth, // GL_DEPTH_COMPONENT
	ettfDepth16, // GL_DEPTH_COMPONENT16
	ettfDepth24, // GL_DEPTH_COMPONENT24
	ettfDepth32, // GL_DEPTH_COMPONENT32
	
	ettf_Depth_32F, // GL_DEPTH_COMPONENT32F
	ettfNV_Depth_32F, // GL_DEPTH_COMPONENT32F_NV
	
	// combined depth+stencil formats
	ettfDepth_Stencil, // GL_DEPTH_STENCIL
	ettfUnsignedInt_24_8, // GL_UNSIGNED_INT_24_8
	ettfDepth24_Stencil8, // GL_DEPTH24_STENCIL8
	
	ettf_DepthF32_Stencil8, // GL_DEPTH32F_STENCIL8
	ettfNV_DepthF32_Stencil8, // GL_DEPTH32F_STENCIL8_NV
	
	// stencil formats
	ettfStencil, // GL_STENCIL_INDEX
	ettfStencil8, // GL_STENCIL_INDEX8
	ettfStencil16, // GL_STENCIL_INDEX16
	
	// dummy entry for the count of formats
	ETTF_COUNT
};

enum eTestWhats{
	etwColor,
	etwDepth,
	etwStencil,
	etwDepthStencil
};

struct sTestTextureFormat{
	GLint format;
	enum pixelFormat;
	enum pixelType;
	int bitsPerPixel;
	int flags;
	const char *name;
	int what;
};

struct sTestCase{
	int target;
	int testFormat;
};

struct sTestFallback{
	int target;
	int fallbackTarget;
};

#define FLAG_NONE			0
#define FLAG_DEPTH			0x1
#define FLAG_DEPTH_FLOAT	0x2
#define FLAG_STENCIL		0x4
#define FLAG_COMPRESSED		0x8

#define HAS_FLAG_DEPTH(flags)		((flags & FLAG_DEPTH) == FLAG_DEPTH)
#define HAS_FLAG_DEPTH_FLOAT(flags)	((flags & FLAG_DEPTH_FLOAT) == FLAG_DEPTH_FLOAT)
#define HAS_FLAG_STENCIL(flags)		((flags & FLAG_STENCIL) == FLAG_STENCIL)
#define HAS_FLAG_COMPRESSED(flags)	((flags & FLAG_COMPRESSED) == FLAG_COMPRESSED)

static const sTestTextureFormat vTestTextureFormats[ETTF_COUNT] = {// convert: (ettf[_\d\w]+)(, //)(GL_[_\d\w]+) => "\3", // \1
	// integer formats
	{GL_R8, GL_RED, GL_UNSIGNED_BYTE, 8, FLAG_NONE, "GL_R8", etwColor}, // ettfR8
	{GL_R16, GL_RED, GL_UNSIGNED_SHORT, 16, FLAG_NONE, "GL_R16", etwColor}, // ettfR16
	{GL_R8_SNORM, GL_RED, GL_BYTE, 8, FLAG_NONE, "GL_R8_SNORM", etwColor}, // ettfR8_S
	{GL_R16_SNORM, GL_RED, GL_SHORT, 16, FLAG_NONE, "GL_R16_SNORM", etwColor}, // ettfR16_S
	
	{GL_RG8, GL_RG, GL_UNSIGNED_BYTE, 16, FLAG_NONE, "GL_RG8", etwColor}, // ettfRG8
	{GL_RG16, GL_RG, GL_UNSIGNED_SHORT, 32, FLAG_NONE, "GL_RG16", etwColor}, // ettfRG16
	{GL_RG8_SNORM, GL_RG, GL_BYTE, 16, FLAG_NONE, "GL_RG8_SNORM", etwColor}, // ettfRG8_S
	{GL_RG16_SNORM, GL_RG, GL_SHORT, 32, FLAG_NONE, "GL_RG16_SNORM", etwColor}, // ettfRG16_S
	
	{GL_R3_G3_B2, GL_RGB, GL_UNSIGNED_BYTE, 8, FLAG_NONE, "GL_R3_G3_B2", etwColor}, // ettfR3_G3_B2
	{GL_RGB4, GL_RGB, GL_UNSIGNED_BYTE, 12, FLAG_NONE, "GL_RGB4", etwColor}, // ettfRGB4
	{GL_RGB5, GL_RGB, GL_UNSIGNED_BYTE, 16, FLAG_NONE, "GL_RGB5", etwColor}, // ettfRGB5
	{GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 24, FLAG_NONE, "GL_RGB8", etwColor}, // ettfRGB8
	{GL_RGB16, GL_RGB, GL_UNSIGNED_SHORT, 24, FLAG_NONE, "GL_RGB16", etwColor}, // ettfRGB16
	{GL_RGB8_SNORM, GL_RGB, GL_BYTE, 24, FLAG_NONE, "GL_RGB8_SNORM", etwColor}, // ettfRGB8_S
	{GL_RGB16_SNORM, GL_RGB, GL_SHORT, 24, FLAG_NONE, "GL_RGB16_SNORM", etwColor}, // ettfRGB16_S
	
	{GL_RGBA2, GL_RGBA, GL_UNSIGNED_BYTE, 8, FLAG_NONE, "GL_RGBA2", etwColor}, // ettfRGBA2
	{GL_RGBA4, GL_RGBA, GL_UNSIGNED_BYTE, 16, FLAG_NONE, "GL_RGBA4", etwColor}, // ettfRGBA4
	{GL_RGB5_A1, GL_RGBA, GL_UNSIGNED_BYTE, 16, FLAG_NONE, "GL_RGB5_A1", etwColor}, // ettfRGB5_A1
	{GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_NONE, "GL_RGBA8", etwColor}, // ettfRGBA8
	{GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_SHORT, 32, FLAG_NONE, "GL_RGB10_A2", etwColor}, // ettfRGB10_A2
	{GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT, 64, FLAG_NONE, "GL_RGBA16", etwColor}, // ettfRGBA16
	{GL_RGBA8_SNORM, GL_RGBA, GL_BYTE, 32, FLAG_NONE, "GL_RGBA8_SNORM", etwColor}, // ettfRGBA8_S
	{GL_RGBA16_SNORM, GL_RGBA, GL_SHORT, 64, FLAG_NONE, "GL_RGBA16_SNORM", etwColor}, // ettfRGBA16_S
	
	// floating point formats
	{GL_R16F, GL_RED, GL_FLOAT, 16, FLAG_NONE, "GL_R16F", etwColor}, // ettfR16F
	{GL_RG16F, GL_RG, GL_FLOAT, 32, FLAG_NONE, "GL_RG16F", etwColor}, // ettfRG16F
	{GL_RGB16F, GL_RGB, GL_FLOAT, 48, FLAG_NONE, "GL_RGB16F", etwColor}, // ettfRGB16F
	{GL_RGBA16F, GL_RGBA, GL_FLOAT, 64, FLAG_NONE, "GL_RGBA16F", etwColor}, // ettfRGBA16F
	
	{GL_R32F, GL_RED, GL_FLOAT, 32, FLAG_NONE, "GL_R32F", etwColor}, // ettfR32F
	{GL_RG32F, GL_RG, GL_FLOAT, 64, FLAG_NONE, "GL_RG32F", etwColor}, // ettfRG32F
	{GL_RGB32F, GL_RGB, GL_FLOAT, 96, FLAG_NONE, "GL_RGB32F", etwColor}, // ettfRGB32F
	{GL_RGBA32F, GL_RGBA, GL_FLOAT, 128, FLAG_NONE, "GL_RGBA32F", etwColor}, // ettfRGBA32F
	
	{GL_RGB_FLOAT16_ATI, GL_RGB, GL_FLOAT, 48, FLAG_NONE, "GL_RGB_FLOAT16_ATI", etwColor}, // ettfAti_RGB_F16
	{GL_RGBA_FLOAT16_ATI, GL_RGBA, GL_FLOAT, 64, FLAG_NONE, "GL_RGBA_FLOAT16_ATI", etwColor}, // ettfAti_RGBA_F16
	
	{GL_RGB_FLOAT32_ATI, GL_RGB, GL_FLOAT, 96, FLAG_NONE, "GL_RGB_FLOAT32_ATI", etwColor}, // ettfAti_RGB_F32
	{GL_RGBA_FLOAT32_ATI, GL_RGBA, GL_FLOAT, 128, FLAG_NONE, "GL_RGBA_FLOAT32_ATI", etwColor}, // ettfAti_RGBA_F32
	
	{GL_FLOAT_R_NV, GL_LUMINANCE, GL_FLOAT, 16, FLAG_NONE, "GL_FLOAT_R_NV", etwColor}, // ettfNV_R_F
	{GL_FLOAT_RG_NV, GL_LUMINANCE_ALPHA, GL_FLOAT, 32, FLAG_NONE, "GL_FLOAT_RG_NV", etwColor}, // ettfNV_RG_F
	{GL_FLOAT_RGB_NV, GL_RGB, GL_FLOAT, 48, FLAG_NONE, "GL_FLOAT_RGB_NV", etwColor}, // ettfNV_RGB_F
	{GL_FLOAT_RGBA_NV, GL_RGBA, GL_FLOAT, 64, FLAG_NONE, "GL_FLOAT_RGBA_NV", etwColor}, // ettfNV_RGBA_F
	
	{GL_FLOAT_R16_NV, GL_LUMINANCE, GL_FLOAT, 16, FLAG_NONE, "GL_FLOAT_R16_NV", etwColor}, // ettfNV_R_F16
	{GL_FLOAT_RG16_NV, GL_LUMINANCE_ALPHA, GL_FLOAT, 32, FLAG_NONE, "GL_FLOAT_RG16_NV", etwColor}, // ettfNV_RG_F16
	{GL_FLOAT_RGB16_NV, GL_RGB, GL_FLOAT, 48, FLAG_NONE, "GL_FLOAT_RGB16_NV", etwColor}, // ettfNV_RGB_F16
	{GL_FLOAT_RGBA16_NV, GL_RGBA, GL_FLOAT, 64, FLAG_NONE, "GL_FLOAT_RGBA16_NV", etwColor}, // ettfNV_RGBA_F16
	
	{GL_FLOAT_R32_NV, GL_LUMINANCE, GL_FLOAT, 32, FLAG_NONE, "GL_FLOAT_R32_NV", etwColor}, // ettfNV_R_F32
	{GL_FLOAT_RG32_NV, GL_LUMINANCE_ALPHA, GL_FLOAT, 64, FLAG_NONE, "GL_FLOAT_RG32_NV", etwColor}, // ettfNV_RG_F32
	{GL_FLOAT_RGB32_NV, GL_RGB, GL_FLOAT, 96, FLAG_NONE, "GL_FLOAT_RGB32_NV", etwColor}, // ettfNV_RGB_F32
	{GL_FLOAT_RGBA32_NV, GL_RGBA, GL_FLOAT, 128, FLAG_NONE, "GL_FLOAT_RGBA32_NV", etwColor}, // ettfNV_RGBA_F32
	
	{GL_R11F_G11F_B10F, GL_RGB, GL_FLOAT, 32, FLAG_NONE, "GL_R11F_G11F_B10F", etwColor}, // ettfRG11B10F
	
	// integral formats
	{GL_R8I, GL_RED_INTEGER, GL_BYTE, 8, FLAG_NONE, "GL_R8I", etwColor}, // ettfR8I
	{GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 8, FLAG_NONE, "GL_R8UI", etwColor}, // ettfR8UI
	{GL_R16I, GL_RED_INTEGER, GL_SHORT, 16, FLAG_NONE, "GL_R16I", etwColor}, // ettfR16I
	{GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 16, FLAG_NONE, "GL_R16UI", etwColor}, // ettfR16UI
	
	{GL_RG8I, GL_RG_INTEGER, GL_BYTE, 16, FLAG_NONE, "GL_RG8I", etwColor}, // ettfRG8I
	{GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE, 16, FLAG_NONE, "GL_RG8UI", etwColor}, // ettfRG8UI
	{GL_RG16I, GL_RG_INTEGER, GL_SHORT, 32, FLAG_NONE, "GL_RG16I", etwColor}, // ettfRG16I
	{GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_SHORT, 32, FLAG_NONE, "GL_RG16UI", etwColor}, // ettfRG16UI
	
	{GL_RGB8I, GL_RGB_INTEGER, GL_BYTE, 24, FLAG_NONE, "GL_RGB8I", etwColor}, // ettfRGB8I
	{GL_RGB8UI, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, 24, FLAG_NONE, "GL_RGB8UI", etwColor}, // ettfRGB8UI
	{GL_RGB16I, GL_RGB_INTEGER, GL_SHORT, 48, FLAG_NONE, "GL_RGB16I", etwColor}, // ettfRGB16I
	{GL_RGB16UI, GL_RGB_INTEGER, GL_UNSIGNED_SHORT, 48, FLAG_NONE, "GL_RGB16UI", etwColor}, // ettfRGB16UI
	
	{GL_RGBA8I, GL_RGBA_INTEGER, GL_BYTE, 32, FLAG_NONE, "GL_RGBA8I", etwColor}, // ettfRGBA8I
	{GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, 32, FLAG_NONE, "GL_RGBA8UI", etwColor}, // ettfRGBA8UI
	{GL_RGBA16I, GL_RGBA_INTEGER, GL_SHORT, 64, FLAG_NONE, "GL_RGBA16I", etwColor}, // ettfRGBA16I
	{GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, 64, FLAG_NONE, "GL_RGBA16UI", etwColor}, // ettfRGBA16UI
	
	// compressed integer formats
	{GL_COMPRESSED_RGB_S3TC_DXT1, GL_RGB, GL_UNSIGNED_BYTE, 24, FLAG_COMPRESSED, "GL_COMPRESSED_RGB_S3TC_DXT1", etwColor}, // ettfCompress_DXT1_RGB
	{GL_COMPRESSED_RGBA_S3TC_DXT1, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_COMPRESSED, "GL_COMPRESSED_RGBA_S3TC_DXT1", etwColor}, // ettfCompress_DXT1_RGBA
	{GL_COMPRESSED_RGBA_S3TC_DXT3, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_COMPRESSED, "GL_COMPRESSED_RGBA_S3TC_DXT3", etwColor}, // ettfCompress_DXT3_RGBA
	{GL_COMPRESSED_RGBA_S3TC_DXT5, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_COMPRESSED, "GL_COMPRESSED_RGBA_S3TC_DXT5", etwColor}, // ettfCompress_DXT5_RGBA
	
	{GL_COMPRESSED_RGB_FXT1_3DFX, GL_RGB, GL_UNSIGNED_BYTE, 24, FLAG_COMPRESSED, "GL_COMPRESSED_RGB_FXT1_3DFX", etwColor}, // ettfCompress_FXT1_RGB
	{GL_COMPRESSED_RGBA_FXT1_3DFX, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_COMPRESSED, "GL_COMPRESSED_RGBA_FXT1_3DFX", etwColor}, // ettfCompress_FXT1_RGBA
	
	{GL_RGB_S3TC, GL_RGB, GL_UNSIGNED_BYTE, 24, FLAG_COMPRESSED, "GL_RGB_S3TC", etwColor}, // ettfCompress_S3TC_RGB
	{GL_RGB4_S3TC, GL_RGB, GL_UNSIGNED_BYTE, 12, FLAG_COMPRESSED, "GL_RGB4_S3TC", etwColor}, // ettfCompress_S3TC_RGB4
	{GL_RGBA_S3TC, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_COMPRESSED, "GL_RGBA_S3TC", etwColor}, // ettfCompress_S3TC_RGBA
	{GL_RGBA4_S3TC, GL_RGBA, GL_UNSIGNED_BYTE, 16, FLAG_COMPRESSED, "GL_RGBA4_S3TC", etwColor}, // ettfCompress_S3TC_RGBA4
	
	{GL_COMPRESSED_RED, GL_RED, GL_UNSIGNED_BYTE, 8, FLAG_COMPRESSED, "GL_COMPRESSED_RED", etwColor}, // ettfCompress_R
	{GL_COMPRESSED_RG, GL_RG, GL_UNSIGNED_BYTE, 16, FLAG_COMPRESSED, "GL_COMPRESSED_RG", etwColor}, // ettfCompress_RG
	{GL_COMPRESSED_RGB, GL_RGB, GL_UNSIGNED_BYTE, 24, FLAG_COMPRESSED, "GL_COMPRESSED_RGB", etwColor}, // ettfCompress_RGB
	{GL_COMPRESSED_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_COMPRESSED, "GL_COMPRESSED_RGBA", etwColor}, // ettfCompress_RGBA
	
	{GL_COMPRESSED_LUMINANCE_LATC1, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, FLAG_COMPRESSED, "GL_COMPRESSED_LUMINANCE_LATC1", etwColor}, // ettfCompress_Lum_LATC1
	{GL_COMPRESSED_SIGNED_LUMINANCE_LATC1, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, FLAG_COMPRESSED, "GL_COMPRESSED_SIGNED_LUMINANCE_LATC1", etwColor}, // ettfCompress_Sig_Lum_LATC1
	{GL_COMPRESSED_LUMINANCE_ALPHA_LATC2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, FLAG_COMPRESSED, "GL_COMPRESSED_LUMINANCE_ALPHA_LATC2", etwColor}, // ettfCompress_Lum_Alpha_LATC2
	{GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, FLAG_COMPRESSED, "GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2", etwColor}, // ettfCompress_Sig_Lum_Alpha_LATC2
	
	{GL_COMPRESSED_RED_RGTC1, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, FLAG_COMPRESSED, "GL_COMPRESSED_RED_RGTC1", etwColor}, // ettfCompress_Red_RGTC1
	{GL_COMPRESSED_SIGNED_RED_RGTC1, GL_LUMINANCE, GL_UNSIGNED_BYTE, 8, FLAG_COMPRESSED, "GL_COMPRESSED_SIGNED_RED_RGTC1", etwColor}, // ettfCompress_Sig_Red_RGTC1
	{GL_COMPRESSED_RED_GREEN_RGTC2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, FLAG_COMPRESSED, "GL_COMPRESSED_RED_GREEN_RGTC2", etwColor}, // ettfCompress_Red_Green_RGTC1
	{GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 16, FLAG_COMPRESSED, "GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2", etwColor}, // ettfCompress_Sig_Red_Green_RGTC1
	
	{GL_COMPRESSED_RGB8_ETC2, GL_RGB, GL_UNSIGNED_BYTE, 24, FLAG_COMPRESSED, "GL_COMPRESSED_RGB8_ETC2", etwColor}, // ettfCompress_ETC2_RGB
	{GL_COMPRESSED_RGBA8_ETC2_EAC, GL_RGBA, GL_UNSIGNED_BYTE, 32, FLAG_COMPRESSED, "GL_COMPRESSED_RGBA8_ETC2_EAC", etwColor}, // ettfCompress_ETC2_RGBA
	
	// depth formats
	{GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, 24, FLAG_DEPTH, "GL_DEPTH_COMPONENT", etwDepth}, // ettfDepth
	{GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT, 16, FLAG_DEPTH, "GL_DEPTH_COMPONENT16", etwDepth}, // ettfDepth16
	{GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, 24, FLAG_DEPTH, "GL_DEPTH_COMPONENT24", etwDepth}, // ettfDepth24
	{GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, 32, FLAG_DEPTH, "GL_DEPTH_COMPONENT32", etwDepth}, // ettfDepth32
	
	{GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, 32, FLAG_DEPTH | FLAG_DEPTH_FLOAT, "GL_DEPTH_COMPONENT32F", etwDepth}, // ettf_Depth_32F
	{GL_DEPTH_COMPONENT32F_NV, GL_DEPTH_COMPONENT, GL_FLOAT, 32, FLAG_DEPTH | FLAG_DEPTH_FLOAT, "GL_DEPTH_COMPONENT32F_NV", etwDepth}, // ettfNV_Depth_32F
	
	// combined depth+stencil formats
	{GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 32, FLAG_DEPTH | FLAG_STENCIL, "GL_DEPTH_STENCIL", etwDepthStencil}, // ettfDepth_Stencil
	{GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 32, FLAG_DEPTH | FLAG_STENCIL, "GL_UNSIGNED_INT_24_8", etwDepthStencil}, // ettfUnsignedInt_24_8
	{GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 32, FLAG_DEPTH | FLAG_STENCIL, "GL_DEPTH24_STENCIL8", etwDepthStencil}, // ettfDepth24_Stencil8
	
	{GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 64,
		FLAG_DEPTH | FLAG_DEPTH_FLOAT | FLAG_STENCIL, "GL_DEPTH32F_STENCIL8", etwDepthStencil}, // ettf_DepthF32_Stencil8
	{GL_DEPTH32F_STENCIL8_NV, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 64,
		FLAG_DEPTH | FLAG_DEPTH_FLOAT | FLAG_STENCIL, "GL_DEPTH32F_STENCIL8_NV", etwDepthStencil}, // ettfNV_DepthF32_Stencil8
	
	// stencil formats
	{GL_STENCIL_INDEX, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, 8, FLAG_STENCIL, "GL_STENCIL_INDEX", etwStencil}, // ettfStencil
	{GL_STENCIL_INDEX8, GL_STENCIL_INDEX8, GL_UNSIGNED_BYTE, 8, FLAG_STENCIL, "GL_STENCIL_INDEX8", etwStencil}, // ettfStencil8
	{GL_STENCIL_INDEX16, GL_STENCIL_INDEX16, GL_UNSIGNED_BYTE, 16, FLAG_STENCIL, "GL_STENCIL_INDEX16", etwStencil}, // ettfStencil16
};

#define TEST_PROGRAM_COUNT 87
static const sTestCase vTestProgram[TEST_PROGRAM_COUNT] = {
	{deoglCapsFmtSupport::eutfR8, ettfR8},
	{deoglCapsFmtSupport::eutfR16, ettfR16},
	{deoglCapsFmtSupport::eutfR8_S, ettfR8_S},
	{deoglCapsFmtSupport::eutfR16_S, ettfR16_S},
	
	{deoglCapsFmtSupport::eutfR8_C, ettfCompress_Red_RGTC1},
	{deoglCapsFmtSupport::eutfR8_C, ettfCompress_Lum_LATC1},
//	{ deoglCapsFmtSupport::eutfR8_C, ettfCompress_R }, // generic format: potentially problematic
	{deoglCapsFmtSupport::eutfR8_C, ettfR8}, // use no compression instead
	
	{deoglCapsFmtSupport::eutfR32F, ettfR32F},
	{deoglCapsFmtSupport::eutfR32F, ettfNV_R_F32},
	{deoglCapsFmtSupport::eutfR32F, ettfNV_R_F},
	
	{deoglCapsFmtSupport::eutfR16F, ettfR16F},
	{deoglCapsFmtSupport::eutfR16F, ettfNV_R_F16},
	
	{deoglCapsFmtSupport::eutfR8I, ettfR8I},
	{deoglCapsFmtSupport::eutfR8UI, ettfR8UI},
	{deoglCapsFmtSupport::eutfR16I, ettfR16I},
	{deoglCapsFmtSupport::eutfR16UI, ettfR16UI},
	
	{deoglCapsFmtSupport::eutfRG8, ettfRG8},
	{deoglCapsFmtSupport::eutfRG16, ettfRG16},
	{deoglCapsFmtSupport::eutfRG8_S, ettfRG8_S},
	{deoglCapsFmtSupport::eutfRG16_S, ettfRG16_S},
	
	{deoglCapsFmtSupport::eutfRG8_C, ettfCompress_Red_Green_RGTC2},
	{deoglCapsFmtSupport::eutfRG8_C, ettfCompress_Lum_Alpha_LATC2},
//	{ deoglCapsFmtSupport::eutfRG8_C, ettfCompress_RG }, // generic format: potentially problematic
	{deoglCapsFmtSupport::eutfRG8_C, ettfRG8}, // use no compression instead
	
	{deoglCapsFmtSupport::eutfRG32F, ettfRG32F},
	{deoglCapsFmtSupport::eutfRG32F, ettfNV_RG_F32},
	{deoglCapsFmtSupport::eutfRG32F, ettfNV_RG_F},
	
	{deoglCapsFmtSupport::eutfRG16F, ettfRG16F},
	{deoglCapsFmtSupport::eutfRG16F, ettfNV_RG_F16},
	
	{deoglCapsFmtSupport::eutfRG8I, ettfRG8I},
	{deoglCapsFmtSupport::eutfRG8UI, ettfRG8UI},
	{deoglCapsFmtSupport::eutfRG16I, ettfRG16I},
	{deoglCapsFmtSupport::eutfRG16UI, ettfRG16UI},
	
	{deoglCapsFmtSupport::eutfR3G3B2, ettfR3_G3_B2},
	{deoglCapsFmtSupport::eutfRGB4, ettfRGB4},
	{deoglCapsFmtSupport::eutfRGB5, ettfRGB5},
	{deoglCapsFmtSupport::eutfRGB8, ettfRGB8},
	{deoglCapsFmtSupport::eutfRGB16, ettfRGB16},
	{deoglCapsFmtSupport::eutfRGB8_S, ettfRGB8_S},
	{deoglCapsFmtSupport::eutfRGB16_S, ettfRGB16_S},
	
#ifdef WITH_OPENGLES
	{deoglCapsFmtSupport::eutfRGB8_C, ettfCompress_ETC2_RGB},
#else
	{deoglCapsFmtSupport::eutfRGB8_C, ettfCompress_DXT1_RGB},
#endif
//	{ deoglCapsFmtSupport::eutfRGB8_C, ettfCompress_RGB }, // generic format: potentially problematic
	{deoglCapsFmtSupport::eutfRGB8_C, ettfRGB8}, // use no compression instead
	
	{deoglCapsFmtSupport::eutfRGB32F, ettfRGB32F},
	{deoglCapsFmtSupport::eutfRGB32F, ettfAti_RGB_F32},
	{deoglCapsFmtSupport::eutfRGB32F, ettfNV_RGB_F32},
	{deoglCapsFmtSupport::eutfRGB32F, ettfNV_RGB_F},
	
	{deoglCapsFmtSupport::eutfRGB16F, ettfRGB16F},
	{deoglCapsFmtSupport::eutfRGB16F, ettfAti_RGB_F16},
	{deoglCapsFmtSupport::eutfRGB16F, ettfNV_RGB_F16},
	
	{deoglCapsFmtSupport::eutfRG11B10F, ettfRG11B10F},
	
	{deoglCapsFmtSupport::eutfRGB8I, ettfRGB8I},
	{deoglCapsFmtSupport::eutfRGB8UI, ettfRGB8UI},
	{deoglCapsFmtSupport::eutfRGB16I, ettfRGB16I},
	{deoglCapsFmtSupport::eutfRGB16UI, ettfRGB16UI},
	
	{deoglCapsFmtSupport::eutfRGBA2, ettfRGBA2},
	{deoglCapsFmtSupport::eutfRGBA4, ettfRGBA4},
	{deoglCapsFmtSupport::eutfRGB5A1, ettfRGB5_A1},
	{deoglCapsFmtSupport::eutfRGBA8, ettfRGBA8},
	{deoglCapsFmtSupport::eutfRGB10A2, ettfRGB10_A2},
	{deoglCapsFmtSupport::eutfRGBA16, ettfRGBA16},
	{deoglCapsFmtSupport::eutfRGBA8_S, ettfRGBA8_S},
	{deoglCapsFmtSupport::eutfRGBA16_S, ettfRGBA16_S},
	
	#ifdef WITH_OPENGLES
	{deoglCapsFmtSupport::eutfRGBA8_C, ettfCompress_ETC2_RGBA},
#else
	{deoglCapsFmtSupport::eutfRGBA8_C, ettfCompress_DXT3_RGBA},
#endif
//	{ deoglCapsFmtSupport::eutfRGBA8_C, ettfCompress_RGBA }, // generic format: potentially problematic
	{deoglCapsFmtSupport::eutfRGBA8_C, ettfRGBA8}, // use no compression instead
	
	{deoglCapsFmtSupport::eutfRGBA32F, ettfRGBA32F},
	{deoglCapsFmtSupport::eutfRGBA32F, ettfAti_RGBA_F32},
	{deoglCapsFmtSupport::eutfRGBA32F, ettfNV_RGBA_F32},
	{deoglCapsFmtSupport::eutfRGBA32F, ettfNV_RGBA_F},
	
	{deoglCapsFmtSupport::eutfRGBA16F, ettfRGBA16F},
	{deoglCapsFmtSupport::eutfRGBA16F, ettfAti_RGBA_F16},
	{deoglCapsFmtSupport::eutfRGBA16F, ettfNV_RGBA_F16},
	
	{deoglCapsFmtSupport::eutfRGBA8I, ettfRGBA8I},
	{deoglCapsFmtSupport::eutfRGBA8UI, ettfRGBA8UI},
	{deoglCapsFmtSupport::eutfRGBA16I, ettfRGBA16I},
	{deoglCapsFmtSupport::eutfRGBA16UI, ettfRGBA16UI},
	
	{deoglCapsFmtSupport::eutfDepth, ettfDepth32},
	{deoglCapsFmtSupport::eutfDepth, ettfDepth24},
	{deoglCapsFmtSupport::eutfDepth, ettfDepth16},
	//{ deoglCapsFmtSupport::eutfDepth, ettfDepth },
	
	{deoglCapsFmtSupport::eutfDepth_Stencil, ettfDepth24_Stencil8},
	{deoglCapsFmtSupport::eutfDepth_Stencil, ettfUnsignedInt_24_8},
	//{ deoglCapsFmtSupport::eutfDepth_Stencil, ettfDepth_Stencil },
	
	{deoglCapsFmtSupport::eutfDepthF, ettf_Depth_32F},
	{deoglCapsFmtSupport::eutfDepthF, ettfNV_Depth_32F},
	
	{deoglCapsFmtSupport::eutfDepthF_Stencil, ettf_DepthF32_Stencil8},
	{deoglCapsFmtSupport::eutfDepthF_Stencil, ettfNV_DepthF32_Stencil8},
	
	{deoglCapsFmtSupport::eutfStencil, ettfStencil8},
	{deoglCapsFmtSupport::eutfStencil, ettfStencil16},
	{deoglCapsFmtSupport::eutfStencil, ettfStencil},
	
	{deoglCapsFmtSupport::eutfDepth16, ettfDepth16}
};

#define TEST_FALLBACK_COUNT 12
static const sTestFallback vTestFallback[TEST_FALLBACK_COUNT] = {
	{deoglCapsFmtSupport::eutfR8_S, deoglCapsFmtSupport::eutfR8},
	{deoglCapsFmtSupport::eutfRG8_S, deoglCapsFmtSupport::eutfRG8},
	{deoglCapsFmtSupport::eutfRGB8_S, deoglCapsFmtSupport::eutfRGB8},
	{deoglCapsFmtSupport::eutfRGBA8_S, deoglCapsFmtSupport::eutfRGBA8},
	
	{deoglCapsFmtSupport::eutfR16F, deoglCapsFmtSupport::eutfR32F},
	{deoglCapsFmtSupport::eutfRG16F, deoglCapsFmtSupport::eutfRG32F},
	{deoglCapsFmtSupport::eutfRGB16F, deoglCapsFmtSupport::eutfRGB32F},
	{deoglCapsFmtSupport::eutfRGBA16F, deoglCapsFmtSupport::eutfRGBA32F},
	
	{deoglCapsFmtSupport::eutfDepth, deoglCapsFmtSupport::eutfDepthF},
	{deoglCapsFmtSupport::eutfDepth_Stencil, deoglCapsFmtSupport::eutfDepthF_Stencil},
	{deoglCapsFmtSupport::eutfDepth, deoglCapsFmtSupport::eutfDepth_Stencil},
	{deoglCapsFmtSupport::eutfDepth16, deoglCapsFmtSupport::eutfDepth}
};

static const char *vTextureFormatNames[deoglCapsFmtSupport::UseTextureFormatCount] = {
	"eutfR8", // eutfR8
	"eutfR16", // eutfR16
	"eutfR8_C", // eutfR8_C
	"eutfR16F", // eutfR16F
	"eutfR32F", // eutfR32F
	"eutfR8I", // eutfR8I
	"eutfR16I", // eutfR16I
	"eutfR8UI", // eutfR8UI
	"eutfR16UI", // eutfR16UI
	"eutfR8_S", // eutfR8_S
	"eutfR16_S", // eutfR16_S
	
	"eutfRG8", // eutfRG8
	"eutfRG16", // eutfRG16
	"eutfRG8_C", // eutfRG8_C
	"eutfRG16F", // eutfRG16F
	"eutfRG32F", // eutfRG32F
	"eutfRG8I", // eutfRG8I
	"eutfRG16I", // eutfRG16I
	"eutfRG8UI", // eutfRG8UI
	"eutfRG16UI", // eutfRG16UI
	"eutfRG8_S", // eutfRG8_S
	"eutfRG16_S", // eutfRG16_S
	
	"eutfR3G3B2", // eutfR3G3B2
	"eutfRGB4", // eutfRGB4
	"eutfRGB5", // eutfRGB5
	"eutfRGB8", // eutfRGB8
	"eutfRGB16", // eutfRGB16
	"eutfRGB8_C", // eutfRGB8_C
	"eutfRG11B10F", // eutfRG11B10F
	"eutfRGB16F", // eutfRGB16F
	"eutfRGB32F", // eutfRGB32F
	"eutfRGB8I", // eutfRGB8I
	"eutfRGB16I", // eutfRGB16I
	"eutfRGB8UI", // eutfRGB8UI
	"eutfRGB16UI", // eutfRGB16UI
	"eutfRGB8_S", // eutfRGB8_S
	"eutfRGB16_S", // eutfRGB16_S
	
	"eutfRGBA2", // eutfRGBA2
	"eutfRGBA4", // eutfRGBA4
	"eutfRGB5A1", // eutfRGB5A1
	"eutfRGBA8", // eutfRGBA8
	"eutfRGB10A2", // eutfRGB10A2
	"eutfRGBA16", // eutfRGBA16
	"eutfRGBA8_C", // eutfRGBA8_C
	"eutfRGBA16F", // eutfRGBA16F
	"eutfRGBA32F", // eutfRGBA32F
	"eutfRGBA8I", // eutfRGBA8I
	"eutfRGBA16I", // eutfRGBA16I
	"eutfRGBA8UI", // eutfRGBA8UI
	"eutfRGBA16UI", // eutfRGBA16UI
	"eutfRGBA8_S", // eutfRGBA8_S
	"eutfRGBA16_S", // eutfRGBA16_S
	
	"eutfDepth", // eutfDepth
	"eutfDepth_Stencil", // eutfDepth_Stencil
	"eutfStencil", // eutfStencil
	"eutfDepth16", // eutfDepth16
	"eutfDepthF", // eutfDepthF
	"eutfDepthF_Stencil" // eutfDepthF_Stencil
};



static bool ENABLE_COMPRESS_LATC1 = false;
static bool ENABLE_COMPRESS_LATC2 = false;
static bool ENABLE_COMPRESS_RGTC1 = false;
static bool ENABLE_COMPRESS_RGTC2 = false;
