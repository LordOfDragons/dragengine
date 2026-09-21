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

#include "deoglArrayCubeMap.h"
#include "../deoglTextureStageManager.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../cubemap/deoglCubeMap.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../memory/deoglMemoryManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTDebug.h"

#ifdef WITH_OPENGLES
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#endif

#include <dragengine/common/exceptions.h>



// Class deoglArrayCubeMap
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglArrayCubeMap::deoglArrayCubeMap(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pSize(1),
pLayerCount(1),
pMemUse(pRenderThread.GetMemoryManager().GetConsumption().textureArrayCube)
{
	pTexture = 0;
	pFormat = &renderThread.GetCapabilities().GetFormats().
		RequireUseArrayTexCubeFormatFor(deoglCapsFmtSupport::eutfRGB8);
	
	pMipMapLevelCount = 0;
	pRealMipMapLevelCount = 0;
	pMipMapped = false;
}

deoglArrayCubeMap::~deoglArrayCubeMap(){
	DestroyTexture();
}



// Management
///////////////

void deoglArrayCubeMap::SetSize(int size, int layerCount){
	if(size < 1 || layerCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(size == pSize && layerCount == pLayerCount){
		return;
	}
	
	DestroyTexture();
	pSize = size;
	pLayerCount = layerCount;
}

void deoglArrayCubeMap::SetFormat(const deoglCapsTextureFormat *format){
	if(!format){
		DETHROW(deeInvalidParam);
	}
	
	if(format != pFormat){
		DestroyTexture();
		pFormat = format;
	}
}

void deoglArrayCubeMap::SetFormatMappingByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber){
	SetFormat(&pRenderThread.GetCapabilities().GetFormats().RequireUseArrayTexCubeFormatFor(formatNumber));
}

void deoglArrayCubeMap::SetFormatFBOByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber){
	SetFormat(&pRenderThread.GetCapabilities().GetFormats().RequireUseFBOArrayTexCubeFormatFor(formatNumber));
}

void deoglArrayCubeMap::SetMipMapped(bool mipmapped){
	if(mipmapped != pMipMapped){
		DestroyTexture();
		pMipMapped = mipmapped;
	}
}

void deoglArrayCubeMap::SetMipMapLevelCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pMipMapLevelCount){
		DestroyTexture();
		pMipMapLevelCount = count;
	}
}



void deoglArrayCubeMap::CreateTexture(){
	if(pTexture){
		return;
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const GLenum glpixelformat = pFormat->GetPixelFormat();
	const GLenum glpixeltype = pFormat->GetPixelType();
	const GLenum glformat = pFormat->GetFormat();
	
	OGL_CHECK(pRenderThread, glGenTextures(1, &pTexture));
	
	if(!pTexture){
		OGL_CHECK(pRenderThread, glGenTextures(1, &pTexture));
		
		if(!pTexture){
			DETHROW(deeInvalidParam);
		}
	}
	
	tsmgr.EnableBareArrayCubeMap(0, *this);
	
	if(pglTexStorage3D){
		if(pMipMapped){
			pRealMipMapLevelCount = pMipMapLevelCount;
			if(pRealMipMapLevelCount == 0){
				pRealMipMapLevelCount = (int)(floorf(log2f((float)pSize)));
			}
		}
		
		try{
			OGL_CHECK(pRenderThread, pglTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY,
				pRealMipMapLevelCount + 1, glformat, pSize, pSize, pLayerCount * 6));
		}catch(const deException &){
			pRenderThread.GetLogger().LogErrorFormat(
				"glTexStorage3D(CubeArr): levelCount=%d format=%s size=%d layers=%d)",
				pRealMipMapLevelCount + 1, pFormat->GetName().GetString(), pSize, pLayerCount);
			pRenderThread.GetLogger().LogErrorFormat("Supported formats: %s",
				pRenderThread.GetCapabilities().GetFormats().SupportedFormatsArrayTexCube().GetString());
			pRenderThread.GetLogger().LogErrorFormat("Supported FBO formats: %s",
				pRenderThread.GetCapabilities().GetFormats().SupportedFormatsFBOArrayTexCube().GetString());
			throw;
		}
		
	}else{
		OGL_CHECK(pRenderThread, pglTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, glformat,
			pSize, pSize, pLayerCount * 6, 0, glpixelformat, glpixeltype, nullptr));
		
		if(pMipMapped){
			pRealMipMapLevelCount = pMipMapLevelCount;
			int size = pSize;
			int i;
			
			if(pRealMipMapLevelCount == 0){
				pRealMipMapLevelCount = (int)(floorf(log2f((float)size)));
			}
			
			for(i=0; i<pRealMipMapLevelCount; i++){
				size = decMath::max(size >> 1, 1);
				OGL_CHECK(pRenderThread, pglTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, i + 1, glformat,
					size, size, pLayerCount * 6, 0, glpixelformat, glpixeltype, nullptr));
			}
		}
	}
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAX_LEVEL, pRealMipMapLevelCount));
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_NONE));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	
	switch(glpixelformat){
	case GL_RED:
	case GL_LUMINANCE:
	case GL_RED_INTEGER:
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_ONE));
		break;
		
	case GL_RG:
	case GL_LUMINANCE_ALPHA:
	case GL_RG_INTEGER:
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_GREEN));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_ZERO));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_ONE));
		break;
		
	default:
		break;
	}
	
	tsmgr.DisableStage(0);
	
	UpdateMemoryUsage();
	pUpdateDebugObjectLabel();
}

void deoglArrayCubeMap::DestroyTexture(){
	if(pTexture){
		pRenderThread.GetDelayedOperations().DeleteOpenGLTexture(pTexture);
		pTexture = 0;
		
		UpdateMemoryUsage();
	}
}

void deoglArrayCubeMap::SetPixels(const deoglPixelBuffer &pixelBuffer){
	SetPixelsLevel(0, pixelBuffer);
}

void deoglArrayCubeMap::SetPixelsLevel(int level, const deoglPixelBuffer &pixelBuffer){
	const int size = GetLevelSize(level);
	
	if(pixelBuffer.GetWidth() != size || pixelBuffer.GetHeight() != size || pixelBuffer.GetDepth() != pLayerCount * 6){
		DETHROW(deeInvalidParam);
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	CreateTexture();
	tsmgr.EnableBareArrayCubeMap(0, *this);
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	
	if(pixelBuffer.GetCompressed()){
		OGL_CHECK(pRenderThread, pglCompressedTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, level,
			0, 0, 0, size, size, pLayerCount * 6, pFormat->GetFormat(), pixelBuffer.GetImageSize(),
			(const GLvoid*)pixelBuffer.GetPointer()));
		
	}else{
		OGL_CHECK(pRenderThread, pglTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, 0, 0, 0,
			size, size, pLayerCount * 6, pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType(),
			(const GLvoid*)pixelBuffer.GetPointer()));
	}
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	
	tsmgr.DisableStage(0);
}

void deoglArrayCubeMap::GetPixels(deoglPixelBuffer &pixelBuffer) const{
	GetPixelsLevel(0, pixelBuffer);
}

void deoglArrayCubeMap::GetPixelsLevel(int level, deoglPixelBuffer &pixelBuffer) const{
	const int size = GetLevelSize(level);
	
	if(pixelBuffer.GetWidth() != size || pixelBuffer.GetHeight() != size || pixelBuffer.GetDepth() != pLayerCount * 6){
		DETHROW(deeInvalidParam);
	}
	if(!pTexture){
		return;
	}
	
	#ifdef WITH_OPENGLES
	// glReadPixels under OpenGL ES does only support GL_RGBA and GL_RGBA_INTEGRAL.
	// if something else is required (for exampel GL_RGB, GL_RG or GL_RED) we have
	// to use a temporary texture containing 4 components and copy over from there.
	// inefficient but the only solution possible
	switch(pixelBuffer.GetFormat()){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfByte3:{
		const deoglPixelBuffer::Ref tempPixBuf(deoglPixelBuffer::Ref::New(
			deoglPixelBuffer::epfByte4, size, size, pLayerCount * 6));
		const int count = size * size;
		int i, j;
		
		GetPixelsLevel(level, tempPixBuf); // get data using temporary 4-channels buffer
		
		switch(pixelBuffer.GetFormat()){
		case deoglPixelBuffer::epfByte1:{
			for(j=0; j<pLayerCount * 6; j++){
				deoglPixelBuffer::sByte1 *dataDest = pixelBuffer.GetPointerByte1() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf->GetPointerByte4() + count;
				for(i=0; i<count; i++){
					dataDest[i].r = dataSrc[i].r;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfByte2:{
			for(j=0; j<pLayerCount * 6; j++){
				deoglPixelBuffer::sByte2 *dataDest = pixelBuffer.GetPointerByte2() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf->GetPointerByte4() + count;
				for(i=0; i<count; i++){
					dataDest[i].r = dataSrc[i].r;
					dataDest[i].g = dataSrc[i].g;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfByte3:{
			for(j=0; j<pLayerCount * 6; j++){
				deoglPixelBuffer::sByte3 *dataDest = pixelBuffer.GetPointerByte3() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf->GetPointerByte4() + count;
				for(i=0; i<count; i++){
					dataDest[i].r = dataSrc[i].r;
					dataDest[i].g = dataSrc[i].g;
					dataDest[i].b = dataSrc[i].b;
				}
			}
			}return;
			
		default:
			return;
		}
		}return;
		
	case deoglPixelBuffer::epfFloat1:
	case deoglPixelBuffer::epfFloat2:
	case deoglPixelBuffer::epfFloat3:{
		const deoglPixelBuffer::Ref tempPixBuf(deoglPixelBuffer::Ref::New(
			deoglPixelBuffer::epfFloat4, size, size, pLayerCount * 6));
		const int count = size * size;
		int i, j;
		
		GetPixelsLevel(level, tempPixBuf); // get data using temporary 4-channels buffer
		
		switch(pixelBuffer.GetFormat()){
		case deoglPixelBuffer::epfFloat1:{
			for(j=0; j<pLayerCount * 6; j++){
				deoglPixelBuffer::sFloat1 *dataDest = pixelBuffer.GetPointerFloat1() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf->GetPointerFloat4() + count;
				for(i=0; i<count; i++){
					dataDest[i].r = dataSrc[i].r;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfFloat2:{
			for(j=0; j<pLayerCount * 6; j++){
				deoglPixelBuffer::sFloat2 *dataDest = pixelBuffer.GetPointerFloat2() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf->GetPointerFloat4() + count;
				for(i=0; i<count; i++){
					dataDest[i].r = dataSrc[i].r;
					dataDest[i].g = dataSrc[i].g;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfFloat3:{
			for(j=0; j<pLayerCount * 6; j++){
				deoglPixelBuffer::sFloat3 *dataDest = pixelBuffer.GetPointerFloat3() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf->GetPointerFloat4() + count;
				for(i=0; i<count; i++){
					dataDest[i].r = dataSrc[i].r;
					dataDest[i].g = dataSrc[i].g;
					dataDest[i].b = dataSrc[i].b;
				}
			}
			}return;
			
		default:
			return;
		}
		}return;
		
	default:
		break;
	}
	
	// this is called if the format is 4 channels
	const GLubyte * const pixelsPtr = (const GLubyte *)pixelBuffer.GetPointer();
	const int faceStride = pixelBuffer.GetLayerStride();
	deoglRTFramebuffer &rtframebuffer = pRenderThread.GetFramebuffer();
	deoglFramebuffer * const oldFbo = rtframebuffer.GetActive();
	int i;
	
	try{
		const deoglFramebufferManager::Usage fbo(
			rtframebuffer.GetManager().GetFBOWithResolution(size, size));
		rtframebuffer.Activate(fbo);
		fbo->DetachAllImages();
		
		const GLenum buffers[1] = {GL_NONE};
		OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
		if(pFormat->GetIsDepth()){
			OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
		}else{
			OGL_CHECK(pRenderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
		}
		
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
		
		for(i=0; i<pLayerCount * 6; i++){
			if(pFormat->GetIsDepth()){
				fbo->AttachDepthArrayCubeMapLayerFaceLevel(this, i, level);
			}else{
				fbo->AttachColorArrayCubeMapLayerFaceLevel(0, this, i, level);
			}
			
			OGL_CHECK(pRenderThread, glReadPixels(0, 0, size, size, pixelBuffer.GetGLPixelFormat(),
				pixelBuffer.GetGLPixelType(), (GLvoid*)(pixelsPtr + faceStride * i)));
		}
		
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
		
		fbo->DetachColorImage(0);
		fbo->DetachDepthImage();
		
		rtframebuffer.Activate(oldFbo);
		
	}catch(const deException &){
		rtframebuffer.Activate(oldFbo);
		throw;
	}
	
	#else
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	tsmgr.EnableBareArrayCubeMap(0, *this);
	OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
	
	OGL_CHECK(pRenderThread, glGetTexImage(GL_TEXTURE_CUBE_MAP_ARRAY, level, pixelBuffer.GetGLPixelFormat(),
		pixelBuffer.GetGLPixelType(), (GLvoid*)pixelBuffer.GetPointer()));
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
	tsmgr.DisableStage(0);
	#endif
}



int deoglArrayCubeMap::GetLevelSize(int level) const{
	DEASSERT_TRUE(level >= 0)
	DEASSERT_TRUE(level <= pRealMipMapLevelCount)
	
	int i, size = pSize;
	for(i=0; i<level; i++){
		size >>= 1;
		if(size < 1){
			size = 1;
		}
	}
	return size;
}

void deoglArrayCubeMap::CreateMipMaps(){
	if(pMipMapped && pMipMapLevelCount == 0){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		
		tsmgr.EnableBareArrayCubeMap(0, *this);
		pglGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
		tsmgr.DisableStage(0);
	}
}



void deoglArrayCubeMap::CopyFrom(const deoglArrayCubeMap &texture, bool withMipMaps){
	CopyFrom(texture, withMipMaps, 0, 0, pSize, pLayerCount, 0, 0, 0, 0);
}

void deoglArrayCubeMap::CopyFrom(const deoglArrayCubeMap &texture, bool withMipMaps, int srcLayer, int destLayer){
	CopyFrom(texture, withMipMaps, srcLayer, destLayer, pSize, 1, 0, 0, 0, 0);
}

void deoglArrayCubeMap::CopyFrom(const deoglArrayCubeMap &texture, bool withMipMaps, int srcLayer, int destLayer,
int size, int layerCount, int srcX, int srcY, int destX, int destY){
	if(destX < 0 || destY < 0 || srcX < 0 || srcY < 0 || srcLayer < 0 || destLayer < 0
	|| destX + size > pSize || destY + size > pSize || destLayer + layerCount > pLayerCount){
		DETHROW(deeInvalidParam);
	}
	
	const int srcSize = texture.pSize;
	const int srcLayerCount = texture.GetLayerCount();
	
	if(srcX + size > srcSize || srcY + size > srcSize || srcLayer + layerCount > srcLayerCount){
		DETHROW(deeInvalidParam);
	}
	
	CreateTexture();
	
	if(withMipMaps && texture.GetMipMapped() && pMipMapped){
		int destMipMapLevelCount = texture.GetMipMapLevelCount();
		int srcMipMapLevelCount = pMipMapLevelCount;
		int i, mipMapLevelCount;
		
		if(destMipMapLevelCount == 0){
			destMipMapLevelCount = (int)(ceilf(log2f((float)pSize))) + 1;
		}
		if(srcMipMapLevelCount == 0){
			srcMipMapLevelCount = (int)(ceilf(log2f((float)srcSize))) + 1;
		}
		mipMapLevelCount = ((srcMipMapLevelCount < destMipMapLevelCount) ? srcMipMapLevelCount : destMipMapLevelCount);
		
		for(i=0; i<mipMapLevelCount; i++){
			if(pglCopyImageSubData){
				pglCopyImageSubData(texture.GetTexture(), GL_TEXTURE_CUBE_MAP_ARRAY, i, srcX, srcY, srcLayer * 6,
					pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, i, destX, destY, destLayer * 6, size, size, layerCount * 6);
				
			}else if(pglCopyImageSubDataNV){
				pglCopyImageSubDataNV(texture.GetTexture(), GL_TEXTURE_CUBE_MAP_ARRAY, i, srcX, srcY, srcLayer * 6,
					pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, i, destX, destY, destLayer * 6, size, size, layerCount * 6);
				
			}else{
				DETHROW(deeInvalidParam);
			}
			
			srcX >>= 1;
			srcY >>= 1;
			destX >>= 1;
			destY >>= 1;
			size >>= 1;
			if(size < 1){
				size = 1;
			}
		}
		
	}else{
		if(pglCopyImageSubData){
			pglCopyImageSubData(texture.GetTexture(), GL_TEXTURE_CUBE_MAP_ARRAY, 0, srcX, srcY, srcLayer * 6,
				pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, 0, destX, destY, destLayer * 6, size, size, layerCount * 6);
			
		}else if(pglCopyImageSubDataNV){
			pglCopyImageSubDataNV(texture.GetTexture(), GL_TEXTURE_CUBE_MAP_ARRAY, 0, srcX, srcY, srcLayer * 6,
				pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, 0, destX, destY, destLayer * 6, size, size, layerCount * 6);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
}

void deoglArrayCubeMap::CopyFrom(const deoglCubeMap &cubemap, bool withMipMaps, int destLayer){
	CopyFrom(cubemap, withMipMaps, destLayer, pSize, 0, 0, 0, 0);
}

void deoglArrayCubeMap::CopyFrom(const deoglCubeMap &cubemap, bool withMipMaps, int destLayer,
	int size, int srcX, int srcY, int destX, int destY){
	if(destX < 0 || destY < 0 || srcX < 0 || srcY < 0 || destLayer < 0
	|| destX + size > pSize || destY + size > pSize || destLayer >= pLayerCount){
		DETHROW(deeInvalidParam);
	}
	
	const int srcSize = cubemap.GetSize();
	
	if(srcX + size > srcSize || srcY + size > srcSize){
		DETHROW(deeInvalidParam);
	}
	
	CreateTexture();
	
	if(withMipMaps && cubemap.GetMipMapped() && pMipMapped){
		const int srcMipMapLevelCount = cubemap.GetRealMipMapLevelCount();
		int i, mipMapLevelCount;
		
		mipMapLevelCount = pRealMipMapLevelCount;
		if(srcMipMapLevelCount < mipMapLevelCount){
			mipMapLevelCount = srcMipMapLevelCount;
		}
		
		for(i=0; i<=mipMapLevelCount; i++){
			if(pglCopyImageSubData){
				pglCopyImageSubData(cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, i, destX, destY, destLayer * 6, size, size, 6);
				
			}else if(pglCopyImageSubDataNV){
				pglCopyImageSubDataNV(cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, i, destX, destY, destLayer * 6, size, size, 6);
				
			}else{
				DETHROW(deeInvalidParam);
			}
			
			srcX >>= 1;
			srcY >>= 1;
			destX >>= 1;
			destY >>= 1;
			
			size >>= 1;
			if(size < 1){
				size = 1;
			}
		}
		
	}else{
		if(pglCopyImageSubData){
			pglCopyImageSubData(cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, 0, destX, destY, destLayer * 6, size, size, 6);
			
		}else if(pglCopyImageSubDataNV){
			pglCopyImageSubDataNV(cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_CUBE_MAP_ARRAY, 0, destX, destY, destLayer * 6, size, size, 6);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
}



void deoglArrayCubeMap::UpdateMemoryUsage(){
	pMemUse.Clear();
	
	if(!pTexture || !pFormat){
		return;
	}
	
	#ifdef WITH_OPENGLES
	pMemUse.SetUncompressed(*pFormat, pSize, pSize, pLayerCount * 6, pRealMipMapLevelCount);
	
	#else
	if(pFormat->GetIsCompressed()){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareArrayCubeMap(0, *this);
		
		GLint isReallyCompressed = 0;
		OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_ARRAY,
			0, GL_TEXTURE_COMPRESSED, &isReallyCompressed));
		
		if(isReallyCompressed){
			unsigned long consumption = 0ull;
			GLint l, compressedSize;
			for(l=0; l<=pRealMipMapLevelCount; l++){
				OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_ARRAY, l,
					GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize));
				consumption += (unsigned long long)compressedSize;
			}
			
			pMemUse.SetCompressed(consumption, *pFormat);
			
		}else{
			pMemUse.SetUncompressed(*pFormat, pSize, pSize, pLayerCount * 6, pMipMapped ? pRealMipMapLevelCount : 0);
		}
		
		tsmgr.DisableStage(0);
		
	}else{
		pMemUse.SetUncompressed(*pFormat, pSize, pSize, pLayerCount * 6, pMipMapped ? pRealMipMapLevelCount : 0);
	}
	#endif
}



// Helper Functions
/////////////////////

void deoglArrayCubeMap::SetMapingFormat(int channels, bool useFloat, bool compressed){
	if(channels == 1){
		if(useFloat){
			SetFormatMappingByNumber(deoglCapsFmtSupport::eutfR16F);
			
		}else{
			if(compressed){
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfR8_C);
				
			}else{
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfR8);
			}
		}
		
	}else if(channels == 2){
		if(useFloat){
			SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRG16F);
			
		}else{
			if(compressed){
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRG8_C);
				
			}else{
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRG8);
			}
		}
		
	}else if(channels == 3){
		if(useFloat){
			SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRGB16F);
			
		}else{
			if(compressed){
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRGB8_C);
				
			}else{
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRGB8);
			}
		}
		
	}else if(channels == 4){
		if(useFloat){
			SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRGBA16F);
			
		}else{
			if(compressed){
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRGBA8_C);
				
			}else{
				SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRGBA8);
			}
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglArrayCubeMap::SetFBOFormat(int channels, bool useFloat){
	if(channels == 1){
		if(useFloat){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR16F);
			
		}else{
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR8);
		}
		
	}else if(channels == 2){
		if(useFloat){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG16F);
			
		}else{
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG8);
		}
		
	}else if(channels == 3){
		if(useFloat){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB16F);
			
		}else{
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB8);
		}
		
	}else if(channels == 4){
		if(useFloat){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA16F);
			
		}else{
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA8);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglArrayCubeMap::SetFBOFormatIntegral(int channels, int bpp, bool useUnsigned){
	if(channels == 1){
		if(bpp == 8){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfR8UI : deoglCapsFmtSupport::eutfR8I);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfR16UI : deoglCapsFmtSupport::eutfR16I);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(channels == 2){
		if(bpp == 8){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfRG8UI : deoglCapsFmtSupport::eutfRG8I);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfRG16UI : deoglCapsFmtSupport::eutfRG16I);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(channels == 3){
		if(bpp == 8){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfRGB8UI : deoglCapsFmtSupport::eutfRGB8I);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfRGB16UI : deoglCapsFmtSupport::eutfRGB16I);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(channels == 4){
		if(bpp == 8){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfRGBA8UI : deoglCapsFmtSupport::eutfRGBA8I);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(useUnsigned ? deoglCapsFmtSupport::eutfRGBA16UI : deoglCapsFmtSupport::eutfRGBA16I);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglArrayCubeMap::SetFBOFormatSNorm(int channels, int bpp){
	if(channels == 1){
		if(bpp == 8){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR8_S);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR16_S);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(channels == 2){
		if(bpp == 8){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG8_S);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG16_S);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(channels == 3){
		if(bpp == 8){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB8_S);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB16_S);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(channels == 4){
		if(bpp == 8){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA8_S);
			
		}else if(bpp == 16){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA16_S);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglArrayCubeMap::SetDepthFormat(bool packedStencil, bool useFloat){
	if(packedStencil){
		if(useFloat){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfDepthF_Stencil);
			
		}else{
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfDepth_Stencil);
		}
		
	}else{
		if(useFloat){
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfDepthF);
			
		}else{
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfDepth);
		}
	}
}

void deoglArrayCubeMap::CreateMatrixForFace(decMatrix &matrix, const decVector &position, int face){
	deoglCubeMap::CreateMatrixForFace(matrix, position, face);
}

void deoglArrayCubeMap::CreateMatrixForFace(decDMatrix &matrix, const decDVector &position, int face){
	deoglCubeMap::CreateMatrixForFace(matrix, position, face);
}

void deoglArrayCubeMap::SetDebugObjectLabel(const char *name){
	pDebugObjectLabel.FormatSafe("ArrCube: {}", name);
	if(pTexture){
		pUpdateDebugObjectLabel();
	}
}

void deoglArrayCubeMap::pUpdateDebugObjectLabel(){
	if(pTexture && pRenderThread.GetDebug().GetEnableHwDebugOutput()){
		pRenderThread.GetDebug().SetDebugObjectLabel(GL_TEXTURE, pTexture, pDebugObjectLabel);
	}
}