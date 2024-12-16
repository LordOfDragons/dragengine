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

#include "devkDevice.h"

enum eTestFormats{
	// integer formats
	etfR8, // VK_FORMAT_R8_UNORM
	etfR16, // VK_FORMAT_R16_UNORM
	etfR8_S, // VK_FORMAT_R8_SNORM
	etfR16_S, // VK_FORMAT_R16_SNORM
	
	etfRG8, // VK_FORMAT_R8G8_UNORM
	etfRG16, // VK_FORMAT_R16G16_UNORM
	etfRG8_S, // VK_FORMAT_R8G8_SNORM
	etfRG16_S, // VK_FORMAT_R16G16_SNORM
	
	etfRGB5, // VK_FORMAT_R5G6B5_UNORM_PACK16
	etfRGB8, // VK_FORMAT_R8G8B8_UNORM
	etfRGB16, // VK_FORMAT_R16G16B16_UNORM
	etfRGB8_S, // VK_FORMAT_R8G8B8_SNORM
	etfRGB16_S, // VK_FORMAT_R16G16B16_SNORM
	
	etfRGBA4, // VK_FORMAT_R4G4B4A4_UNORM_PACK16
	etfRGB5_A1, // VK_FORMAT_R5G5B5A1_UNORM_PACK16
	etfRGBA8, // VK_FORMAT_R8G8B8A8_UNORM
	etfRGB10_A2, // VK_FORMAT_A2B10G10R10_UNORM_PACK32
	etfRGBA16, // VK_FORMAT_R16G16B16A16_UNORM
	etfRGBA8_S, // VK_FORMAT_R8G8B8A8_SNORM
	etfRGBA16_S, // VK_FORMAT_R16G16B16A16_SNORM
	
	// floating point formats
	etfR16F, // VK_FORMAT_R16_SFLOAT
	etfRG16F, // VK_FORMAT_R16G16_SFLOAT
	etfRGB16F, // VK_FORMAT_R16G16B16_SFLOAT
	etfRGBA16F, // VK_FORMAT_R16G16B16A16_SFLOAT
	
	etfR32F, // VK_FORMAT_R32_SFLOAT
	etfRG32F, // VK_FORMAT_R32G32_SFLOAT
	etfRGB32F, // VK_FORMAT_R32G32B32_SFLOAT
	etfRGBA32F, // VK_FORMAT_R32G32B32A32_SFLOAT
	
	etfRG11B10F, // VK_FORMAT_B10G11R11_UFLOAT_PACK32
	
	// integral formats
	etfR8I, // VK_FORMAT_R8_SINT
	etfR8UI, // VK_FORMAT_R8_UINT
	etfR16I, // VK_FORMAT_R16_SINT
	etfR16UI, // VK_FORMAT_R16_UINT
	
	etfRG8I, // VK_FORMAT_R8G8_SINT
	etfRG8UI, // VK_FORMAT_R8G8_UINT
	etfRG16I, // VK_FORMAT_R16G16_SINT
	etfRG16UI, // VK_FORMAT_R16G16_UINT
	
	etfRGB8I, // VK_FORMAT_R8G8B8_SINT
	etfRGB8UI, // VK_FORMAT_R8G8B8_UINT
	etfRGB16I, // VK_FORMAT_R16G16B16_SINT
	etfRGB16UI, // VK_FORMAT_R16G16B16_UINT
	
	etfRGBA8I, // VK_FORMAT_R8G8B8A8_SINT
	etfRGBA8UI, // VK_FORMAT_R8G8B8A8_UINT
	etfRGBA16I, // VK_FORMAT_R16G16B16A16_SINT
	etfRGBA16UI, // VK_FORMAT_R16G16B16A16_UINT
	
	// compressed integer formats
	etfCompress_DXT1_RGB, // VK_FORMAT_BC1_RGB_UNORM_BLOCK
	etfCompress_DXT1_RGBA, // VK_FORMAT_BC1_RGBA_UNORM_BLOCK
	etfCompress_DXT3_RGBA, // VK_FORMAT_BC3_UNORM_BLOCK
	etfCompress_DXT4_R, // VK_FORMAT_BC4_UNORM_BLOCK
	etfCompress_DXT5_RG, // VK_FORMAT_BC5_UNORM_BLOCK
	etfCompress_DXT6_RGBF, // VK_FORMAT_BC6H_SFLOAT_BLOCK
	etfCompress_DXT7_RGBA, // VK_FORMAT_BC7_UNORM_BLOCK
	
	etfCompress_ASTC4_RGBA, // VK_FORMAT_ASTC_4x4_UNORM_BLOCK (bpp 8, lowest quality)
	etfCompress_ASTC5_RGBA, // VK_FORMAT_ASTC_5x5_UNORM_BLOCK (bpp 5.12)
	etfCompress_ASTC6_RGBA, // VK_FORMAT_ASTC_6x6_UNORM_BLOCK (bpp 3.56)
	etfCompress_ASTC8_RGBA, // VK_FORMAT_ASTC_8x8_UNORM_BLOCK (bpp 2.00)
	etfCompress_ASTC10_RGBA, // VK_FORMAT_ASTC_10x10_UNORM_BLOCK (bpp 1.28)
	etfCompress_ASTC12_RGBA, // VK_FORMAT_ASTC_12x12_UNORM_BLOCK (bpp 0.89, highest quality)
	
	/*
	etfCompress_ASTC4_RGBAF, // VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK (bpp 8, lowest quality)
	etfCompress_ASTC5_RGBAF, // VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK (bpp 5.12)
	etfCompress_ASTC6_RGBAF, // VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK (bpp 3.56)
	etfCompress_ASTC8_RGBAF, // VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK (bpp 2.00)
	etfCompress_ASTC10_RGBAF, // VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK (bpp 1.28)
	etfCompress_ASTC12_RGBAF, // VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK (bpp 0.89, highest quality)
	*/
	
	// depth formats
	etfDepth16, // VK_FORMAT_D16_UNORM
	etfDepth32F, // VK_FORMAT_D32_SFLOAT
	
	// combined depth+stencil formats
	etfDepth24_Stencil8, // VK_FORMAT_D24_UNORM_S8_UINT
	etfDepth32F_Stencil8, // VK_FORMAT_D32_SFLOAT_S8_UINT
	
	// stencil formats
	etfStencil8, // VK_FORMAT_S8_UINT
	
	// dummy entry for the count of formats
	etf_COUNT
};

enum eTestWhats{
	etwColor,
	etwDepth,
	etwStencil,
	etwDepthStencil
};

struct sTestCase{
	devkDevice::eFormats target;
	eTestFormats testFormat;
};

struct sTestTextureFormat{
	VkFormat format;
	int bitsPerPixel;
	int flags;
	const char *name;
	eTestWhats what;
};

struct sTestFallback{
	devkDevice::eFormats target;
	devkDevice::eFormats fallbackTarget;
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

static const sTestTextureFormat vTestTextureFormats[etf_COUNT] = {
	// convert: (etf[_\d\w]+)(, // )(VK_[_\d\w]+) => {$3, 8, FLAG_NONE, "$3", etwColor}, // $1
	
	// integer formats
	{VK_FORMAT_R8_UNORM, 8, FLAG_NONE, "VK_FORMAT_R8_UNORM", etwColor}, // etfR8
	{VK_FORMAT_R16_UNORM, 16, FLAG_NONE, "VK_FORMAT_R16_UNORM", etwColor}, // etfR16
	{VK_FORMAT_R8_SNORM, 8, FLAG_NONE, "VK_FORMAT_R8_SNORM", etwColor}, // etfR8_S
	{VK_FORMAT_R16_SNORM, 16, FLAG_NONE, "VK_FORMAT_R16_SNORM", etwColor}, // etfR16_S
	
	{VK_FORMAT_R8G8_UNORM, 16, FLAG_NONE, "VK_FORMAT_R8G8_UNORM", etwColor}, // etfRG8
	{VK_FORMAT_R16G16_UNORM, 32, FLAG_NONE, "VK_FORMAT_R16G16_UNORM", etwColor}, // etfRG16
	{VK_FORMAT_R8G8_SNORM, 16, FLAG_NONE, "VK_FORMAT_R8G8_SNORM", etwColor}, // etfRG8_S
	{VK_FORMAT_R16G16_SNORM, 32, FLAG_NONE, "VK_FORMAT_R16G16_SNORM", etwColor}, // etfRG16_S
	
	{VK_FORMAT_R5G6B5_UNORM_PACK16, 16, FLAG_NONE, "VK_FORMAT_R5G6B5_UNORM_PACK16", etwColor}, // etfRGB5
	{VK_FORMAT_R8G8B8_UNORM, 24, FLAG_NONE, "VK_FORMAT_R8G8B8_UNORM", etwColor}, // etfRGB8
	{VK_FORMAT_R16G16B16_UNORM, 48, FLAG_NONE, "VK_FORMAT_R16G16B16_UNORM", etwColor}, // etfRGB16
	{VK_FORMAT_R8G8B8_SNORM, 24, FLAG_NONE, "VK_FORMAT_R8G8B8_SNORM", etwColor}, // etfRGB8_S
	{VK_FORMAT_R16G16B16_SNORM, 48, FLAG_NONE, "VK_FORMAT_R16G16B16_SNORM", etwColor}, // etfRGB16_S
	
	{VK_FORMAT_R4G4B4A4_UNORM_PACK16, 16, FLAG_NONE, "VK_FORMAT_R4G4B4A4_UNORM_PACK16", etwColor}, // etfRGBA4
	{VK_FORMAT_R5G5B5A1_UNORM_PACK16, 16, FLAG_NONE, "VK_FORMAT_R5G5B5A1_UNORM_PACK16", etwColor}, // etfRGB5_A1
	{VK_FORMAT_R8G8B8A8_UNORM, 32, FLAG_NONE, "VK_FORMAT_R8G8B8A8_UNORM", etwColor}, // etfRGBA8
	{VK_FORMAT_A2B10G10R10_UNORM_PACK32, 32, FLAG_NONE, "VK_FORMAT_A2B10G10R10_UNORM_PACK32", etwColor}, // etfRGB10_A2
	{VK_FORMAT_R16G16B16A16_UNORM, 64, FLAG_NONE, "VK_FORMAT_R16G16B16A16_UNORM", etwColor}, // etfRGBA16
	{VK_FORMAT_R8G8B8A8_SNORM, 32, FLAG_NONE, "VK_FORMAT_R8G8B8A8_SNORM", etwColor}, // etfRGBA8_S
	{VK_FORMAT_R16G16B16A16_SNORM, 64, FLAG_NONE, "VK_FORMAT_R16G16B16A16_SNORM", etwColor}, // etfRGBA16_S
	
	// floating point formats
	{VK_FORMAT_R16_SFLOAT, 16, FLAG_NONE, "VK_FORMAT_R16_SFLOAT", etwColor}, // etfR16F
	{VK_FORMAT_R16G16_SFLOAT, 32, FLAG_NONE, "VK_FORMAT_R16G16_SFLOAT", etwColor}, // etfRG16F
	{VK_FORMAT_R16G16B16_SFLOAT, 48, FLAG_NONE, "VK_FORMAT_R16G16B16_SFLOAT", etwColor}, // etfRGB16F
	{VK_FORMAT_R16G16B16A16_SFLOAT, 64, FLAG_NONE, "VK_FORMAT_R16G16B16A16_SFLOAT", etwColor}, // etfRGBA16F
	
	{VK_FORMAT_R32_SFLOAT, 32, FLAG_NONE, "VK_FORMAT_R32_SFLOAT", etwColor}, // etfR32F
	{VK_FORMAT_R32G32_SFLOAT, 64, FLAG_NONE, "VK_FORMAT_R32G32_SFLOAT", etwColor}, // etfRG32F
	{VK_FORMAT_R32G32B32_SFLOAT, 96, FLAG_NONE, "VK_FORMAT_R32G32B32_SFLOAT", etwColor}, // etfRGB32F
	{VK_FORMAT_R32G32B32A32_SFLOAT, 128, FLAG_NONE, "VK_FORMAT_R32G32B32A32_SFLOAT", etwColor}, // etfRGBA32F
	
	{VK_FORMAT_B10G11R11_UFLOAT_PACK32, 32, FLAG_NONE, "VK_FORMAT_B10G11R11_UFLOAT_PACK32", etwColor}, // etfRG11B10F
	
	// integral formats
	{VK_FORMAT_R8_SINT, 8, FLAG_NONE, "VK_FORMAT_R8_SINT", etwColor}, // etfR8I
	{VK_FORMAT_R8_UINT, 8, FLAG_NONE, "VK_FORMAT_R8_UINT", etwColor}, // etfR8UI
	{VK_FORMAT_R16_SINT, 16, FLAG_NONE, "VK_FORMAT_R16_SINT", etwColor}, // etfR16I
	{VK_FORMAT_R16_UINT, 16, FLAG_NONE, "VK_FORMAT_R16_UINT", etwColor}, // etfR16UI
	
	{VK_FORMAT_R8G8_SINT, 16, FLAG_NONE, "VK_FORMAT_R8G8_SINT", etwColor}, // etfRG8I
	{VK_FORMAT_R8G8_UINT, 16, FLAG_NONE, "VK_FORMAT_R8G8_UINT", etwColor}, // etfRG8UI
	{VK_FORMAT_R16G16_SINT, 32, FLAG_NONE, "VK_FORMAT_R16G16_SINT", etwColor}, // etfRG16I
	{VK_FORMAT_R16G16_UINT, 32, FLAG_NONE, "VK_FORMAT_R16G16_UINT", etwColor}, // etfRG16UI
	
	{VK_FORMAT_R8G8B8_SINT, 24, FLAG_NONE, "VK_FORMAT_R8G8B8_SINT", etwColor}, // etfRGB8I
	{VK_FORMAT_R8G8B8_UINT, 24, FLAG_NONE, "VK_FORMAT_R8G8B8_UINT", etwColor}, // etfRGB8UI
	{VK_FORMAT_R16G16B16_SINT, 48, FLAG_NONE, "VK_FORMAT_R16G16B16_SINT", etwColor}, // etfRGB16I
	{VK_FORMAT_R16G16B16_UINT, 48, FLAG_NONE, "VK_FORMAT_R16G16B16_UINT", etwColor}, // etfRGB16UI
	
	{VK_FORMAT_R8G8B8A8_SINT, 32, FLAG_NONE, "VK_FORMAT_R8G8B8A8_SINT", etwColor}, // etfRGBA8I
	{VK_FORMAT_R8G8B8A8_UINT, 32, FLAG_NONE, "VK_FORMAT_R8G8B8A8_UINT", etwColor}, // etfRGBA8UI
	{VK_FORMAT_R16G16B16A16_SINT, 64, FLAG_NONE, "VK_FORMAT_R16G16B16A16_SINT", etwColor}, // etfRGBA16I
	{VK_FORMAT_R16G16B16A16_UINT, 64, FLAG_NONE, "VK_FORMAT_R16G16B16A16_UINT", etwColor}, // etfRGBA16UI
	
	// compressed integer formats
	{VK_FORMAT_BC1_RGB_UNORM_BLOCK, 24, FLAG_COMPRESSED, "VK_FORMAT_BC1_RGB_UNORM_BLOCK", etwColor}, // etfCompress_DXT1_RGB
	{VK_FORMAT_BC1_RGBA_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_BC1_RGBA_UNORM_BLOCK", etwColor}, // etfCompress_DXT1_RGBA
	{VK_FORMAT_BC3_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_BC3_UNORM_BLOCK", etwColor}, // etfCompress_DXT3_RGBA
	{VK_FORMAT_BC4_SNORM_BLOCK, 8, FLAG_COMPRESSED, "VK_FORMAT_BC4_SNORM_BLOCK", etwColor}, // etfCompress_DXT4_R
	{VK_FORMAT_BC5_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_BC5_UNORM_BLOCK", etwColor}, // etfCompress_DXT5_RG
	{VK_FORMAT_BC6H_SFLOAT_BLOCK, 24, FLAG_COMPRESSED, "VK_FORMAT_BC6H_SFLOAT_BLOCK", etwColor}, // etfCompress_DXT6_RGBF
	{VK_FORMAT_BC7_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_BC7_UNORM_BLOCK", etwColor}, // etfCompress_DXT7_RGBA
	
	{VK_FORMAT_ASTC_4x4_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_ASTC_4x4_UNORM_BLOCK", etwColor}, // etfCompress_ASTC4_RGBA (bpp 8, lowest quality)
	{VK_FORMAT_ASTC_5x5_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_ASTC_5x5_UNORM_BLOCK", etwColor}, // etfCompress_ASTC5_RGBA (bpp 5.12)
	{VK_FORMAT_ASTC_6x6_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_ASTC_6x6_UNORM_BLOCK", etwColor}, // etfCompress_ASTC6_RGBA (bpp 3.56)
	{VK_FORMAT_ASTC_8x8_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_ASTC_8x8_UNORM_BLOCK", etwColor}, // etfCompress_ASTC8_RGBA (bpp 2.00)
	{VK_FORMAT_ASTC_10x10_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_ASTC_10x10_UNORM_BLOCK", etwColor}, // etfCompress_ASTC10_RGBA (bpp 1.28)
	{VK_FORMAT_ASTC_12x12_UNORM_BLOCK, 32, FLAG_COMPRESSED, "VK_FORMAT_ASTC_12x12_UNORM_BLOCK", etwColor}, // etfCompress_ASTC12_RGBA (bpp 0.89, highest quality)
	
	/*
	{VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK, 8, FLAG_COMPRESSED, "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK", etwColor}, // etfCompress_ASTC4_RGBAF (bpp 8, lowest quality)
	{VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK, 8, FLAG_COMPRESSED, "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK", etwColor}, // etfCompress_ASTC5_RGBAF (bpp 5.12)
	{VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK, 8, FLAG_COMPRESSED, "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK", etwColor}, // etfCompress_ASTC6_RGBAF (bpp 3.56)
	{VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK, 8, FLAG_COMPRESSED, "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK", etwColor}, // etfCompress_ASTC8_RGBAF (bpp 2.00)
	{VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK, 8, FLAG_COMPRESSED, "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK", etwColor}, // etfCompress_ASTC10_RGBAF (bpp 1.28)
	{VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK, 8, FLAG_COMPRESSED, "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK", etwColor}, // etfCompress_ASTC12_RGBAF (bpp 0.89, highest quality)
	*/
	
	// depth formats
	{VK_FORMAT_D16_UNORM, 16, FLAG_DEPTH, "VK_FORMAT_D16_UNORM", etwDepth}, // etfDepth16
	{VK_FORMAT_D32_SFLOAT, 32, FLAG_DEPTH_FLOAT, "VK_FORMAT_D32_SFLOAT", etwDepth}, // etfDepth32F
	
	// combined depth+stencil formats
	{VK_FORMAT_D24_UNORM_S8_UINT, 32, FLAG_DEPTH | FLAG_STENCIL, "VK_FORMAT_D24_UNORM_S8_UINT", etwDepthStencil}, // etfDepth24_Stencil8
	{VK_FORMAT_D32_SFLOAT_S8_UINT, 48, FLAG_DEPTH_FLOAT | FLAG_STENCIL, "VK_FORMAT_D32_SFLOAT_S8_UINT", etwDepthStencil}, // etfDepth32F_Stencil8
	
	// stencil formats
	{VK_FORMAT_S8_UINT, 8, FLAG_STENCIL, "VK_FORMAT_S8_UINT", etwStencil} // etfStencil8
};

#define TEST_PROGRAM_COUNT 69
static const sTestCase vTestProgram[TEST_PROGRAM_COUNT] = {
	{devkDevice::efR8, etfR8},
	{devkDevice::efR16, etfR16},
	{devkDevice::efR8_S, etfR8_S},
	{devkDevice::efR16_S, etfR16_S},
	
	{devkDevice::efR8_C, etfCompress_DXT4_R},
	{devkDevice::efR8_C, etfR8}, // no compression
	
	{devkDevice::efR32F, etfR32F},
	
	{devkDevice::efR16F, etfR16F},
	
	{devkDevice::efR8I, etfR8I},
	{devkDevice::efR8UI, etfR8UI},
	{devkDevice::efR16I, etfR16I},
	{devkDevice::efR16UI, etfR16UI},
	
	{devkDevice::efRG8, etfRG8},
	{devkDevice::efRG16, etfRG16},
	{devkDevice::efRG8_S, etfRG8_S},
	{devkDevice::efRG16_S, etfRG16_S},
	
	{devkDevice::efRG8_C, etfCompress_DXT5_RG},
	{devkDevice::efRG8_C, etfRG8}, // no compression
	
	{devkDevice::efRG32F, etfRG32F},
	
	{devkDevice::efRG16F, etfRG16F},
	
	{devkDevice::efRG8I, etfRG8I},
	{devkDevice::efRG8UI, etfRG8UI},
	{devkDevice::efRG16I, etfRG16I},
	{devkDevice::efRG16UI, etfRG16UI},
	
	{devkDevice::efRGB4, etfRGBA4},
	{devkDevice::efRGB5, etfRGB5},
	{devkDevice::efRGB8, etfRGB8},
	{devkDevice::efRGB16, etfRGB16},
	{devkDevice::efRGB8_S, etfRGB8_S},
	{devkDevice::efRGB16_S, etfRGB16_S},
	
	{devkDevice::efRGB8_C, etfCompress_DXT1_RGB},
	{devkDevice::efRGB8_C, etfRGB8}, // no compression
	
	{devkDevice::efRGB32F, etfRGB32F},
	
	{devkDevice::efRGB16F, etfRGB16F},
	
	{devkDevice::efRG11B10F, etfRG11B10F},
	
	{devkDevice::efRGB8I, etfRGB8I},
	{devkDevice::efRGB8UI, etfRGB8UI},
	{devkDevice::efRGB16I, etfRGB16I},
	{devkDevice::efRGB16UI, etfRGB16UI},
	
	{devkDevice::efRGBA4, etfRGBA4},
	{devkDevice::efRGB5A1, etfRGB5_A1},
	{devkDevice::efRGBA8, etfRGBA8},
	{devkDevice::efRGB10A2, etfRGB10_A2},
	{devkDevice::efRGBA16, etfRGBA16},
	{devkDevice::efRGBA8_S, etfRGBA8_S},
	{devkDevice::efRGBA16_S, etfRGBA16_S},
	
	{devkDevice::efRGBA8_C, etfCompress_DXT7_RGBA},
	{devkDevice::efRGBA8_C, etfCompress_DXT3_RGBA},
	{devkDevice::efRGBA8_C, etfCompress_DXT1_RGBA},
	{devkDevice::efRGBA8_C, etfCompress_ASTC6_RGBA},
	{devkDevice::efRGBA8_C, etfCompress_ASTC8_RGBA},
	{devkDevice::efRGBA8_C, etfCompress_ASTC4_RGBA},
	{devkDevice::efRGBA8_C, etfCompress_ASTC10_RGBA},
	{devkDevice::efRGBA8_C, etfCompress_ASTC12_RGBA},
	{devkDevice::efRGBA8_C, etfRGBA8}, // use no compression instead
	
	{devkDevice::efRGBA32F, etfRGBA32F},
	
	{devkDevice::efRGBA16F, etfRGBA16F},
	
	{devkDevice::efRGBA8I, etfRGBA8I},
	{devkDevice::efRGBA8UI, etfRGBA8UI},
	{devkDevice::efRGBA16I, etfRGBA16I},
	{devkDevice::efRGBA16UI, etfRGBA16UI},
	
	{devkDevice::efDepth16, etfDepth16},
	
	{devkDevice::efDepthF, etfDepth32F},
	
	{devkDevice::efStencil, etfStencil8},
	
	{devkDevice::efDepthF_Stencil, etfDepth32F_Stencil8},
	
	{devkDevice::efDepth, etfDepth16},
	{devkDevice::efDepth, etfDepth32F},
	
	{devkDevice::efDepth_Stencil, etfDepth24_Stencil8},
	{devkDevice::efDepth_Stencil, etfDepth32F_Stencil8}
};

// see https://docs.vulkan.org/spec/latest/chapters/formats.html#features-required-format-support

#define TEST_FALLBACK_COUNT 30
static const sTestFallback vTestFallback[TEST_FALLBACK_COUNT] = {
	// RGBA
	{devkDevice::efRGBA4, devkDevice::efRGBA8},
	{devkDevice::efRGB5A1, devkDevice::efRGBA8},
	{devkDevice::efRGB10A2, devkDevice::efRGBA16UI},
	{devkDevice::efRGBA16, devkDevice::efRGBA16UI},
	{devkDevice::efRGBA8_S, devkDevice::efRGBA8},
	{devkDevice::efRGBA16_S, devkDevice::efRGBA16I},
	
	{devkDevice::efRGBA8_C, devkDevice::efRGBA8}, // use no compression instead
	
	// RGB
	{devkDevice::efRGB32F, devkDevice::efRGBA32F},
	
	{devkDevice::efRGB16F, devkDevice::efRGBA16F},
	
	{devkDevice::efRG11B10F, devkDevice::efRGB16F},
	
	{devkDevice::efRGB4, devkDevice::efRGB5},
	{devkDevice::efRGB8, devkDevice::efRGBA8},
	{devkDevice::efRGB16, devkDevice::efRGBA16},
	{devkDevice::efRGB8_S, devkDevice::efRGBA8_S},
	{devkDevice::efRGB16_S, devkDevice::efRGBA16_S},
	
	{devkDevice::efRGB8I, devkDevice::efRGBA8I},
	{devkDevice::efRGB8UI, devkDevice::efRGBA8UI},
	{devkDevice::efRGB16I, devkDevice::efRGBA16I},
	{devkDevice::efRGB16UI, devkDevice::efRGBA16UI},
	
	{devkDevice::efRGB8_C, devkDevice::efRGB8}, // no compression
	
	// RG
	{devkDevice::efRG8_S, devkDevice::efRG8},
	{devkDevice::efRG16, devkDevice::efRG16UI},
	{devkDevice::efRG16_S, devkDevice::efRG16I},
	
	{devkDevice::efRG8_C, devkDevice::efRG8}, // no compression
	
	// R
	{devkDevice::efR16, devkDevice::efR16UI},
	{devkDevice::efR8_S, devkDevice::efR8},
	{devkDevice::efR16_S, devkDevice::efR16I},
	
	{devkDevice::efR8_C, devkDevice::efR8}, // no compression
	
	// depth, stencil
	{devkDevice::efDepth16, devkDevice::efDepthF},
	
	{devkDevice::efDepth, devkDevice::efDepth_Stencil}
};

static const char *vTextureFormatNames[devkDevice::FormatCount] = {
	"efR8", // efR8
	"efR16", // efR16
	"efR8_C", // efR8_C
	"efR16F", // efR16F
	"efR32F", // efR32F
	"efR8I", // efR8I
	"efR16I", // efR16I
	"efR8UI", // efR8UI
	"efR16UI", // efR16UI
	"efR8_S", // efR8_S
	"efR16_S", // efR16_S
	
	"efRG8", // efRG8
	"efRG16", // efRG16
	"efRG8_C", // efRG8_C
	"efRG16F", // efRG16F
	"efRG32F", // efRG32F
	"efRG8I", // efRG8I
	"efRG16I", // efRG16I
	"efRG8UI", // efRG8UI
	"efRG16UI", // efRG16UI
	"efRG8_S", // efRG8_S
	"efRG16_S", // efRG16_S
	
	"efRGB4", // efRGB4
	"efRGB5", // efRGB5
	"efRGB8", // efRGB8
	"efRGB16", // efRGB16
	"efRGB8_C", // efRGB8_C
	"efRG11B10F", // efRG11B10F
	"efRGB16F", // efRGB16F
	"efRGB32F", // efRGB32F
	"efRGB8I", // efRGB8I
	"efRGB16I", // efRGB16I
	"efRGB8UI", // efRGB8UI
	"efRGB16UI", // efRGB16UI
	"efRGB8_S", // efRGB8_S
	"efRGB16_S", // efRGB16_S
	
	"efRGBA4", // efRGBA4
	"efRGB5A1", // efRGB5A1
	"efRGBA8", // efRGBA8
	"efRGB10A2", // efRGB10A2
	"efRGBA16", // efRGBA16
	"efRGBA8_C", // efRGBA8_C
	"efRGBA16F", // efRGBA16F
	"efRGBA32F", // efRGBA32F
	"efRGBA8I", // efRGBA8I
	"efRGBA16I", // efRGBA16I
	"efRGBA8UI", // efRGBA8UI
	"efRGBA16UI", // efRGBA16UI
	"efRGBA8_S", // efRGBA8_S
	"efRGBA16_S", // efRGBA16_S
	
	"efDepth", // efDepth
	"efDepth_Stencil", // efDepth_Stencil
	"efStencil", // efStencil
	"efDepth16", // efDepth16
	"efDepthF", // efDepthF
	"efDepthF_Stencil" // efDepthF_Stencil
};
