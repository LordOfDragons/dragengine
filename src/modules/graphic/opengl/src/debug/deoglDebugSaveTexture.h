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

#ifndef _DEOGLDEBUGSAVETEXTURE_H_
#define _DEOGLDEBUGSAVETEXTURE_H_

#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImage.h>

class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglArrayTexture;



/**
 * Debug Save Texture.
 * Helper class saving textures of various kinds to files for debugging purpose.
 * All images are stored in the format /&lt;base-path&gt;/&lt;name&gt;_&lt;timestamp&gt;.xxx
 * where xxx is a file extension depending on the texture to be saved. Time-stamp is
 * in the format YYYY-MM-DD_HH-MM-SS with YYYY the year, MM the month, DD the day,
 * HH the hours, MM the minutes and SS the seconds and is optional.
 */
class deoglDebugSaveTexture{
public:
	/** Convertion. */
	enum eConvertions{
		/** No conversion, color as is. */
		ecNoConversion,
		/** Linear color to sRGB color. */
		ecColorLinear2sRGB,
		/** Linear color to tone mapped sRGB color. */
		ecColorLinearToneMapsRGB,
		/** Linear color scaled by 10 to sRGB color. */
		ecColorLinearScale10,
		/** Conversion suitable for depth buffer values. */
		ecDepthBuffer,
		
		/** Conversion suitable for inverse depth buffer values. */
		ecDepthBufferInverse,
		
		/** Conversion of normals into encoded normals. */
		ecNormal,
		/** NaN and Inf check. */
		ecNaNInf,
		/** Log-Intensity. */
		ecLogIntensity,
		/** Number of conversions. */
		EC_COUNT
	};
	
	/** Depth types. */
	enum eDepthTypes{
		/** Linear. */
		edtLinear,
		
		/** Depth. */
		edtDepth,
		
		/** Linear inverse. */
		edtLinearInverse,
		
		/** Depth inverse. */
		edtDepthInverse
	};
	
private:
	deoglRenderThread &pRenderThread;
	decString pBasePath;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new debug save texture object. */
	deoglDebugSaveTexture(deoglRenderThread &renderThread);
	/** Cleans up the debug save texture object. */
	~deoglDebugSaveTexture();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the base path where images are stored under. */
	inline const decString &GetBasePath() const{return pBasePath;}
	/** Sets the base path where images are stored under. */
	void SetBasePath(const char *path);
	
	/** Saves a texture to file. */
	void SaveTexture(deoglTexture &texture, const char *name);
	/** Saves a level of a texture to file. */
	void SaveTextureLevel(deoglTexture &texture, int level, const char *name);
	/** Saves a texture to file with conversion. */
	void SaveTextureConversion(deoglTexture &texture, const char *name, eConvertions conversion);
	/** Saves a level of a texture to file with conversion. */
	void SaveTextureLevelConversion(deoglTexture &texture, int level, const char *name, eConvertions conversion);
	/** Saves a depth texture to file. */
	void SaveDepthTexture(deoglTexture &texture, const char *name, eDepthTypes type);
	/** Saves a depth texture level to file. */
	void SaveDepthTextureLevel(deoglTexture &texture, int level, const char *name, eDepthTypes type);
	/** Saves a stencil texture to file. */
	void SaveStencilTexture(deoglTexture &texture, const char *name);
	/** Saves a stencil texture to file. */
	void SaveStencilArrayTexture(deoglArrayTexture &texture, const char *name);
	
	/** Saves a cube map to file. */
	void SaveCubeMap(deoglCubeMap &cubemap, const char *name, bool upsideDown);
	/** Saves a level of a cubemap to file. */
	void SaveCubeMapLevel(deoglCubeMap &cubemap, int level, const char *name, bool upsideDown);
	/** Saves a cubemap to file with conversion. */
	void SaveCubeMapConversion(deoglCubeMap &cubemap, const char *name, bool upsideDown, eConvertions conversion);
	/** Saves a level of a cubemap to file with conversion. */
	void SaveCubeMapLevelConversion(deoglCubeMap &cubemap, int level, const char *name, bool upsideDown, eConvertions conversion);
	
	/** Saves a depth cube map to file. */
	void SaveDepthCubeMap(deoglCubeMap &cubemap, const char *name, bool linearDepth);
	void SaveDepthCubeMapLevel(deoglCubeMap &cubemap, int level, const char *name, bool linearDepth);
	
	/** Saves an array texture to file. */
	void SaveArrayTexture(deoglArrayTexture &texture, const char *name);
	/** Saves a level of an array texture to file. */
	void SaveArrayTextureLevel(deoglArrayTexture &texture, int level, const char *name);
	/** Saves array texture to file with conversion. */
	void SaveArrayTextureConversion(deoglArrayTexture &texture, const char *name, eConvertions conversion);
	/** Saves level of an array texture to file with conversion. */
	void SaveArrayTextureLevelConversion(deoglArrayTexture &texture, int level, const char *name, eConvertions conversion);
	/** Saves an array texture to file. */
	void SaveDepthArrayTexture(deoglArrayTexture &texture, const char *name, bool linearDepth);
	/** Saves an array texture to file. */
	void SaveDepthArrayTextureLevel(deoglArrayTexture &texture, int level, const char *name, bool linearDepth);
	
	/** Retrieves the pixel buffer type opengl pixel format and type. */
	deoglPixelBuffer::ePixelFormats GetPixelBufferType(GLenum &pixelFormat, GLenum &pixelType) const;
	/** Retrieves the stride for a pixel buffer format. */
	int GetStride(int width, int pixelBufferType) const;
	/** Component count for pixel buffer type. */
	int GetComponentCount(int pixelBufferType);
	/** Bit count for pixel buffer type. */
	int GetBitCount(int pixelBufferType);
	/*@}*/
	
private:
	void pConvertDataRGBA(const deoglPixelBuffer::sFloat4 *inputData, sRGBA8 *outputData,
		int width, int height, bool yflip, eConvertions conversion);
};

#endif
