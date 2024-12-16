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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglTexture.h"
#include "../deoglTextureStageManager.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../memory/deoglMemoryManager.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTDebug.h"

#ifdef OS_ANDROID
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#endif

#include <dragengine/common/exceptions.h>



// Class deoglTexture
///////////////////////

// Constructor, destructor
////////////////////////////

deoglTexture::deoglTexture(deoglRenderThread &renderThread) :
pRenderThread(renderThread),

#ifdef WITH_OPENGL
pGlTexture(0),
pGlFormat(renderThread.GetCapabilities().GetFormats().GetUseTex2DFormatFor(deoglCapsFmtSupport::eutfRGB8)),
#endif

#ifdef WITH_VULKAN
pVkFormat(nullptr),
#endif

pMipMapLevelCount(0),
pRealMipMapLevelCount(0),
pMipMapped(false),
pMemUse(pRenderThread.GetMemoryManager().GetConsumption().texture2D){
}

deoglTexture::~deoglTexture(){
	DestroyTexture();
}



// Management
///////////////

#ifdef WITH_OPENGL

void deoglTexture::SetFormat(const deoglCapsTextureFormat *format){
	DEASSERT_NOTNULL(format)
	if(format == pGlFormat){
		return;
	}
	
	DestroyTexture();
	pGlFormat = format;
}

void deoglTexture::SetFormatMappingByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber){
	SetFormat(pRenderThread.GetCapabilities().GetFormats().GetUseTex2DFormatFor(formatNumber));
}

void deoglTexture::SetFormatFBOByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber){
	SetFormat(pRenderThread.GetCapabilities().GetFormats().GetUseFBOTex2DFormatFor(formatNumber));
}

#endif


#ifdef WITH_VULKAN

void deoglTexture::SetFormat(const devkFormat *format){
	DEASSERT_NOTNULL(format)
	if(format == pVkFormat){
		return;
	}
	
	DestroyTexture();
	pVkFormat = format;
}

void deoglTexture::SetFormatMappingByNumber(devkDevice::eFormats format){
	SetFormat(pRenderThread.GetVulkanDevice()->GetUseTexFormat(format));
}

void deoglTexture::SetFormatFBOByNumber(devkDevice::eFormats format){
	SetFormat(pRenderThread.GetVulkanDevice()->GetUseFboFormat(format));
}

#endif


void deoglTexture::SetSize(int width, int height){
	SetSize(decPoint(width, height));
}

void deoglTexture::SetSize(const decPoint &size){
	DEASSERT_TRUE(size > decPoint())
	if(pSize == size){
		return;
	}
	
	DestroyTexture();
	pSize = size;
}

void deoglTexture::SetMipMapped(bool mipmapped){
	if(mipmapped == pMipMapped){
		return;
	}
	
	DestroyTexture();
	pMipMapped = mipmapped;
}

void deoglTexture::SetMipMapLevelCount(int count){
	DEASSERT_TRUE(count >= 0)
	if(count == pMipMapLevelCount){
		return;
	}
	
	DestroyTexture();
	pMipMapLevelCount = count;
}



void deoglTexture::CreateTexture(){
	if(pGlTexture){
		return;
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const GLenum glpixelformat = pGlFormat->GetPixelFormat();
	const GLenum glpixeltype = pGlFormat->GetPixelType();
	const GLenum glformat = pGlFormat->GetFormat();
	
	OGL_CHECK(pRenderThread, glGenTextures(1, &pGlTexture));
	
	if(! pGlTexture){
		OGL_CHECK(pRenderThread, glGenTextures(1, &pGlTexture));
		DEASSERT_NOTNULL(pGlTexture)
	}
	
	tsmgr.EnableBareTexture(0, *this);
	
	OGL_CHECK(pRenderThread, glTexImage2D(GL_TEXTURE_2D, 0, glformat,
		pSize.x, pSize.y, 0, glpixelformat, glpixeltype, NULL));
	
	if(pMipMapped){
		int count = pMipMapLevelCount;
		decPoint size(pSize);
		int i;
		
		if(count == 0){
			count = (int)(floorf(log2f((float)decMath::max(size.x, size.y))));
		}
		
		for(i=0; i<count; i++){
			size = decPoint(size.x >> 1, size.y >> 1).Largest(decPoint(1, 1));
			OGL_CHECK(pRenderThread, glTexImage2D(GL_TEXTURE_2D, i + 1,
				glformat, size.x, size.y, 0, glpixelformat, glpixeltype, NULL));
		}
		
		pRealMipMapLevelCount = count;
	}
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, pRealMipMapLevelCount));
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	
	switch(glpixelformat){
	case GL_RED:
	case GL_LUMINANCE:
	case GL_RED_INTEGER:
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE));
		break;
		
	case GL_RG:
	case GL_LUMINANCE_ALPHA:
	case GL_RG_INTEGER:
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED));
		OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN));
		break;
		
	default:
		break;
	}
	
	//OGL_CHECK(pRenderThread, glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST)); // GL_FATEST // GL_DONT_CARE
	
	/*
	if(pIsDepth){
		GLint depth;
		OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH_SIZE, &depth));
		printf("a depth texture (%u) has been created with size (%i,%i) and depth %i\n", pTexture, pSize.x, pSize.y, depth);
	}
	*/
	tsmgr.DisableStage(0);
	
	/*
	int result;
	OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED, &result));
	pOgl->LogInfoFormat("Texture %p: compressed=%i", this, result);
	*/
	
	UpdateMemoryUsage();
	pUpdateDebugObjectLabel();
}

void deoglTexture::DestroyTexture(){
	if(pGlTexture){
		pRenderThread.GetDelayedOperations().DeleteOpenGLTexture(pGlTexture);
		pGlTexture = 0;
		
		UpdateMemoryUsage();
	}
}

void deoglTexture::SetPixels(const deoglPixelBuffer &pixelBuffer){
	SetPixelsLevel(0, pixelBuffer);
}

void deoglTexture::SetPixelsLevel(int level, const deoglPixelBuffer &pixelBuffer){
	DEASSERT_TRUE(pixelBuffer.GetDepth() == 1)
	SetPixelsLevelLayer(level, pixelBuffer, 0);
}

void deoglTexture::SetPixelsLevelLayer(int level, const deoglPixelBuffer &pixelBuffer, int layer){
	int width, height;
	GetLevelSize(level, width, height);
	
	DEASSERT_TRUE(pixelBuffer.GetWidth() == width)
	DEASSERT_TRUE(pixelBuffer.GetHeight() == height)
	DEASSERT_TRUE(layer >= 0)
	DEASSERT_TRUE(layer < pixelBuffer.GetDepth())
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	CreateTexture();
	tsmgr.EnableBareTexture(0, *this);
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	
	const char * const pixelBufferData = (const char *)pixelBuffer.GetPointer()
		+ pixelBuffer.GetLayerStride() * layer;
	
	if(pixelBuffer.GetCompressed()){
		OGL_CHECK(pRenderThread, pglCompressedTexImage2D(GL_TEXTURE_2D, level, pGlFormat->GetFormat(),
			width, height, 0, pixelBuffer.GetImageSize(), (const GLvoid *)pixelBufferData));
		//OGL_CHECK(pRenderThread, pglCompressedTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height,
		//	pFormat->GetFormat(), pixels.GetImageSize(), (const GLvoid *)pixelBufferData));
		
	}else{
#ifdef OS_ANDROID
		oglClearError();
		pglTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, pixelBuffer.GetGLPixelFormat(),
			pixelBuffer.GetGLPixelType(), (const GLvoid *)pixelBufferData);
		if(glGetError() == GL_INVALID_OPERATION){
			pRenderThread.GetLogger().LogInfoFormat(
				"deoglTexture::SetPixelsLevelLayer Failed: size(%d,%d) level=%d pf=%d pt=%d",
				width, height, level, pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType());
		}
#else
		OGL_CHECK(pRenderThread, pglTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height,
			pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType(), (const GLvoid *)pixelBufferData));
#endif
	}
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	
	tsmgr.DisableStage(0);
}

void deoglTexture::GetPixels(deoglPixelBuffer &pixelBuffer) const{
	GetPixelsLevel(0, pixelBuffer);
}

void deoglTexture::GetPixelsLevel(int level, deoglPixelBuffer &pixelBuffer) const{
	int width, height;
	GetLevelSize(level, width, height);
	
	/*if(pixelBuffer.GetWidth() != width || pixelBuffer.GetHeight() != height || pixelBuffer.GetDepth() != 1){
		pRenderThread.GetLogger().LogInfoFormat("PROBLEM! level=%d width(%d=%d) height(%d=%d) depth(%d)",
			level, pixelBuffer.GetWidth(), width, pixelBuffer.GetHeight(), height, pixelBuffer.GetDepth());
		DETHROW(deeInvalidParam);
	}*/
	if(!pGlTexture){
		return;
	}
	
#ifdef OS_ANDROID
	// glReadPixels under OpenGL ES does only support GL_RGBA and GL_RGBA_INTEGRAL.
	// if something else is required (for exampel GL_RGB, GL_RG or GL_RED) we have
	// to use a temporary texture containing 4 components and copy over from there.
	// inefficient but the only solution possible
	switch(pixelBuffer.GetFormat()){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfByte3:{
		const deoglPixelBuffer::Ref tempPixBuf(deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer(deoglPixelBuffer::epfByte4, width, height, 1)));
		const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf->GetPointerByte4();
		const int count = width * height;
		int i;
		
		GetPixelsLevel(level, tempPixBuf); // get data using temporary 4-channels buffer
		
		switch(pixelBuffer.GetFormat()){
		case deoglPixelBuffer::epfByte1:{
			deoglPixelBuffer::sByte1 *dataDest = pixelBuffer.GetPointerByte1();
			for(i=0; i<count; i++){
				dataDest[i].r = dataSrc[i].r;
			}
			}return;
			
		case deoglPixelBuffer::epfByte2:{
			deoglPixelBuffer::sByte2 *dataDest = pixelBuffer.GetPointerByte2();
			for(i=0; i<count; i++){
				dataDest[i].r = dataSrc[i].r;
				dataDest[i].g = dataSrc[i].g;
			}
			}return;
			
		case deoglPixelBuffer::epfByte3:{
			deoglPixelBuffer::sByte3 *dataDest = pixelBuffer.GetPointerByte3();
			for(i=0; i<count; i++){
				dataDest[i].r = dataSrc[i].r;
				dataDest[i].g = dataSrc[i].g;
				dataDest[i].b = dataSrc[i].b;
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
			new deoglPixelBuffer(deoglPixelBuffer::epfFloat4, width, height, 1)));
		const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf->GetPointerFloat4();
		const int count = width * height;
		int i;
		
		GetPixelsLevel(level, tempPixBuf); // get data using temporary 4-channels buffer
		
		switch(pixelBuffer.GetFormat()){
		case deoglPixelBuffer::epfFloat1:{
			deoglPixelBuffer::sFloat1 *dataDest = pixelBuffer.GetPointerFloat1();
			for(i=0; i<count; i++){
				dataDest[i].r = dataSrc[i].r;
			}
			}return;
			
		case deoglPixelBuffer::epfFloat2:{
			deoglPixelBuffer::sFloat2 *dataDest = pixelBuffer.GetPointerFloat2();
			for(i=0; i<count; i++){
				dataDest[i].r = dataSrc[i].r;
				dataDest[i].g = dataSrc[i].g;
			}
			}return;
			
		case deoglPixelBuffer::epfFloat3:{
			deoglPixelBuffer::sFloat3 *dataDest = pixelBuffer.GetPointerFloat3();
			for(i=0; i<count; i++){
				dataDest[i].r = dataSrc[i].r;
				dataDest[i].g = dataSrc[i].g;
				dataDest[i].b = dataSrc[i].b;
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
	deoglRTFramebuffer &rtframebuffer = pRenderThread.GetFramebuffer();
	deoglFramebuffer * const oldFbo = rtframebuffer.GetActive();
	deoglFramebuffer *fbo = NULL;
	try{
		fbo = rtframebuffer.GetManager().GetFBOWithResolution(width, height);
		//fbo = new deoglFramebuffer(pRenderThread, false);
		rtframebuffer.Activate(fbo);
		fbo->DetachAllImages();
		
		const GLenum buffers[1] = { GL_NONE };
		OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
		if(pFormat->GetIsDepth()){
			fbo->AttachDepthTextureLevel((deoglTexture*)this, level);
			OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
			
		}else{
			fbo->AttachColorTextureLevel(0, (deoglTexture*)this, level);
			OGL_CHECK(pRenderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
		}
		//fbo->Verify(); // temporary
		
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
		OGL_CHECK(pRenderThread, glReadPixels(0, 0, width, height, pixelBuffer.GetGLPixelFormat(),
			pixelBuffer.GetGLPixelType(), (GLvoid*)pixelBuffer.GetPointer()));
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
		
		fbo->DetachColorImage(0);
		fbo->DetachDepthImage();
		
		rtframebuffer.Activate(oldFbo);
		
		fbo->DecreaseUsageCount();
		//delete fbo;
		fbo = NULL;
		
	}catch(const deException &){
		rtframebuffer.Activate(oldFbo);
		if(fbo){
			fbo->DecreaseUsageCount();
			//delete fbo;
		}
		throw;
	}
	
#else
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	tsmgr.EnableBareTexture(0, *this);
	OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
	
	OGL_CHECK(pRenderThread, glGetTexImage(GL_TEXTURE_2D, level, pixelBuffer.GetGLPixelFormat(),
		pixelBuffer.GetGLPixelType(), (GLvoid*)pixelBuffer.GetPointer()));
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
	tsmgr.DisableStage(0);
#endif
}



void deoglTexture::GetLevelSize(int level, int &width, int &height) const{
	DEASSERT_TRUE(level >= 0)
	DEASSERT_TRUE(level <= pRealMipMapLevelCount)
	
	int i;
	
	width = pSize.x;
	height = pSize.y;
	
	for(i=0; i<level; i++){
		width = decMath::max(width >> 1, 1);
		height = decMath::max(height >> 1, 1);
	}
}

void deoglTexture::CreateMipMaps(){
	if(!pMipMapped || pMipMapLevelCount > 0){
		return;
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	tsmgr.EnableBareTexture(0, *this);
	pglGenerateMipmap(GL_TEXTURE_2D);
	tsmgr.DisableStage(0);
}



void deoglTexture::CopyFrom(const deoglTexture &texture, bool withMipMaps){
	CopyFrom(texture, withMipMaps, pSize.x, pSize.y, 0, 0, 0, 0);
}

void deoglTexture::CopyFrom(const deoglTexture &texture, bool withMipMaps,
int width, int height, int srcX, int srcY, int destX, int destY){
	DEASSERT_TRUE(destX >= 0)
	DEASSERT_TRUE(destY >= 0)
	DEASSERT_TRUE(srcX >= 0)
	DEASSERT_TRUE(srcY >= 0)
	DEASSERT_TRUE(destX + width <= pSize.x)
	DEASSERT_TRUE(destY + height <= pSize.y)
	
	const int srcHeight = texture.GetHeight();
	const int srcWidth = texture.GetWidth();
	
	DEASSERT_TRUE(srcX + width <= srcWidth)
	DEASSERT_TRUE(srcY + height <= srcHeight)
	
	CreateTexture();
	
	if(withMipMaps && texture.GetMipMapped() && pMipMapped){
		const int srcMipMapLevelCount = texture.GetRealMipMapLevelCount();
		int i, mipMapLevelCount;
		
		mipMapLevelCount = pRealMipMapLevelCount;
		if(srcMipMapLevelCount < mipMapLevelCount){
			mipMapLevelCount = srcMipMapLevelCount;
		}
		
		for(i=0; i<mipMapLevelCount; i++){
			if(pglCopyImageSubData){
				pglCopyImageSubData(texture.GetGlTexture(), GL_TEXTURE_2D, i, srcX, srcY, 0,
					pGlTexture, GL_TEXTURE_2D, i, destX, destY, 0, width, height, 1);
				
			}else if(pglCopyImageSubDataNV){
				pglCopyImageSubDataNV(texture.GetGlTexture(), GL_TEXTURE_2D, i, srcX, srcY, 0,
					pGlTexture, GL_TEXTURE_2D, i, destX, destY, 0, width, height, 1);
				
			}else{
				DETHROW(deeInvalidParam);
			}
			
			srcX >>= 1;
			srcY >>= 1;
			destX >>= 1;
			destY >>= 1;
			width = decMath::max(width >> 1, 1);
			height = decMath::max(height >> 1, 1);
		}
		
	}else{
		if(pglCopyImageSubData){
			pglCopyImageSubData(texture.GetGlTexture(), GL_TEXTURE_2D, 0, srcX, srcY, 0,
				pGlTexture, GL_TEXTURE_2D, 0, destX, destY, 0, width, height, 1);
			
		}else if(pglCopyImageSubDataNV){
			pglCopyImageSubDataNV(texture.GetGlTexture(), GL_TEXTURE_2D, 0, srcX, srcY, 0,
				pGlTexture, GL_TEXTURE_2D, 0, destX, destY, 0, width, height, 1);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
}



void deoglTexture::UpdateMemoryUsage(){
	pMemUse.Clear();
	
	if(! pGlTexture || ! pGlFormat){
		return;
	}
	
#ifdef OS_ANDROID
	pMemUse.SetUncompressed(*pFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
	
#else
	if(pGlFormat->GetIsCompressed()){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareTexture(0, *this);
		
		GLint isReallyCompressed = 0;
		OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_2D,
			0, GL_TEXTURE_COMPRESSED, &isReallyCompressed));
		
		if(isReallyCompressed){
			unsigned long consumption = 0ull;
			GLint compressedSize, l;
			for(l=0; l<pRealMipMapLevelCount; l++){
				OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_2D, l,
					GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize));
				consumption += (unsigned long long)compressedSize;
			}
			
			pMemUse.SetCompressed(consumption, *pGlFormat);
			
		}else{
			pMemUse.SetUncompressed(*pGlFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
		}
		
		tsmgr.DisableStage(0);
		
	}else{
		pMemUse.SetUncompressed(*pGlFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
	}
#endif
}



// Helper Functions
/////////////////////

void deoglTexture::SetMapingFormat(int channels, bool useFloat, bool compressed){
#if WITH_OPENGL
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
#endif
	
#if WITH_VULKAN
	if(pRenderThread.GetVulkanDevice()){
		if(channels == 1){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efR16F);
				
			}else{
				if(compressed){
					SetFormatMappingByNumber(devkDevice::efR8_C);
					
				}else{
					SetFormatMappingByNumber(devkDevice::efR8);
				}
			}
			
		}else if(channels == 2){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efRG16F);
				
			}else{
				if(compressed){
					SetFormatMappingByNumber(devkDevice::efRG8_C);
					
				}else{
					SetFormatMappingByNumber(devkDevice::efRG8);
				}
			}
			
		}else if(channels == 3){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efRGB16F);
				
			}else{
				if(compressed){
					SetFormatMappingByNumber(devkDevice::efRGB8_C);
					
				}else{
					SetFormatMappingByNumber(devkDevice::efRGB8);
				}
			}
			
		}else if(channels == 4){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efRGBA16F);
				
			}else{
				if(compressed){
					SetFormatMappingByNumber(devkDevice::efRGBA8_C);
					
				}else{
					SetFormatMappingByNumber(devkDevice::efRGBA8);
				}
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
#endif
}

void deoglTexture::SetFBOFormat(int channels, bool useFloat){
#ifdef WITH_OPENGL
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
#endif
	
#ifdef WITH_VULKAN
	if(pRenderThread.GetVulkanDevice()){
		if(channels == 1){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efR16F);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efR8);
			}
			
		}else if(channels == 2){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efRG16F);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRG8);
			}
			
		}else if(channels == 3){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efRGB16F);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRGB8);
			}
			
		}else if(channels == 4){
			if(useFloat){
				SetFormatMappingByNumber(devkDevice::efRGBA16F);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRGBA8);
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
#endif
}

void deoglTexture::SetFBOFormatFloat32(int channels){
#ifdef WITH_OPENGL
	if(channels == 1){
		SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR32F);
		
	}else if(channels == 2){
		SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG32F);
		
	}else if(channels == 3){
		SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB32F);
		
	}else if(channels == 4){
		SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA32F);
		
	}else{
		DETHROW(deeInvalidParam);
	}
#endif
	
#ifdef WITH_VULKAN
	if(channels == 1){
		SetFormatMappingByNumber(devkDevice::efR32F);
		
	}else if(channels == 2){
		SetFormatMappingByNumber(devkDevice::efRG32F);
		
	}else if(channels == 3){
		SetFormatMappingByNumber(devkDevice::efRGB32F);
		
	}else if(channels == 4){
		SetFormatMappingByNumber(devkDevice::efRGBA32F);
		
	}else{
		DETHROW(deeInvalidParam);
	}
#endif
}

void deoglTexture::SetFBOFormatIntegral(int channels, int bpp, bool useUnsigned){
#ifdef WITH_OPENGL
	if(bpp == 8){
		if(channels == 1){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR8UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR8I);
			}
			
		}else if(channels == 2){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG8UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG8I);
			}
			
		}else if(channels == 3){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB8UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB8I);
			}
			
		}else if(channels == 4){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA8UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA8I);
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(bpp == 16){
		if(channels == 1){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR16UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR16I);
			}
			
		}else if(channels == 2){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG16UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG16I);
			}
			
		}else if(channels == 3){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB16UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB16I);
			}
			
		}else if(channels == 4){
			if(useUnsigned){
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA16UI);
				
			}else{
				SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA16I);
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
#endif
	
#ifdef WITH_VULKAN
	if(bpp == 8){
		if(channels == 1){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efR8UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efR8I);
			}
			
		}else if(channels == 2){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efRG8UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRG8I);
			}
			
		}else if(channels == 3){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efRGB8UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRGB8I);
			}
			
		}else if(channels == 4){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efRGBA8UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRGBA8I);
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(bpp == 16){
		if(channels == 1){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efR16UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efR16I);
			}
			
		}else if(channels == 2){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efRG16UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRG16I);
			}
			
		}else if(channels == 3){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efRGB16UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRGB16I);
			}
			
		}else if(channels == 4){
			if(useUnsigned){
				SetFormatMappingByNumber(devkDevice::efRGBA16UI);
				
			}else{
				SetFormatMappingByNumber(devkDevice::efRGBA16I);
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
#endif
}

void deoglTexture::SetFBOFormatSNorm(int channels, int bpp){
#ifdef WITH_OPENGL
	switch(bpp){
	case 8:
		switch(channels){
		case 1:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR8_S);
			break;
			
		case 2:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG8_S);
			break;
			
		case 3:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB8_S);
			break;
			
		case 4:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA8_S);
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		break;
		
	case 16:
		switch(channels){
		case 1:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfR16_S);
			break;
			
		case 2:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRG16_S);
			break;
			
		case 3:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGB16_S);
			break;
			
		case 4:
			SetFormatFBOByNumber(deoglCapsFmtSupport::eutfRGBA16_S);
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
#endif
	
#ifdef WITH_VULKAN
	switch(bpp){
	case 8:
		switch(channels){
		case 1:
			SetFormatMappingByNumber(devkDevice::efR8_S);
			break;
			
		case 2:
			SetFormatMappingByNumber(devkDevice::efRG8_S);
			break;
			
		case 3:
			SetFormatMappingByNumber(devkDevice::efRGB8_S);
			break;
			
		case 4:
			SetFormatMappingByNumber(devkDevice::efRGBA8_S);
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		break;
		
	case 16:
		switch(channels){
		case 1:
			SetFormatMappingByNumber(devkDevice::efR16_S);
			break;
			
		case 2:
			SetFormatMappingByNumber(devkDevice::efRG16_S);
			break;
			
		case 3:
			SetFormatMappingByNumber(devkDevice::efRGB16_S);
			break;
			
		case 4:
			SetFormatMappingByNumber(devkDevice::efRGBA16_S);
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
#endif
}

void deoglTexture::SetDepthFormat(bool packedStencil, bool useFloat){
#ifdef WITH_OPENGL
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
#endif
	
#ifdef WITH_VULKAN
	if(packedStencil){
		if(useFloat){
			SetFormatMappingByNumber(devkDevice::efDepthF_Stencil);
			
		}else{
			SetFormatMappingByNumber(devkDevice::efDepth_Stencil);
		}
		
	}else{
		if(useFloat){
			SetFormatMappingByNumber(devkDevice::efDepthF);
			
		}else{
			SetFormatMappingByNumber(devkDevice::efDepth);
		}
	}
#endif
}

void deoglTexture::SetDebugObjectLabel(const char *name){
	pDebugObjectLabel.Format("2D: %s", name);
	if(pGlTexture){
		pUpdateDebugObjectLabel();
	}
}

void deoglTexture::pUpdateDebugObjectLabel(){
	pRenderThread.GetDebug().SetDebugObjectLabel(GL_TEXTURE, pGlTexture, pDebugObjectLabel);
}
