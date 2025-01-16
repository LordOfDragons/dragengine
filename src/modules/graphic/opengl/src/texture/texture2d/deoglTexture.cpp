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

#ifdef BACKEND_OPENGL
#elif defined BACKEND_VULKAN
#include <devkFormat.h>
#include <queue/devkCommandBuffer.h>
#endif

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

#ifdef BACKEND_OPENGL
pTexture(0),
pFormat(0),

#elif defined BACKEND_VULKAN
pFormat(nullptr),
#endif

pMipMapLevelCount(0),
pRealMipMapLevelCount(0),
pMipMapped(false),
pMemUse(pRenderThread.GetMemoryManager().GetConsumption().texture2D)
{
	SetMapingFormat(3, false, false);
}

deoglTexture::~deoglTexture(){
	DestroyTexture();
}



// Management
///////////////

#ifdef BACKEND_OPENGL

void deoglTexture::SetFormat(const deoglCapsTextureFormat *format){
	DEASSERT_NOTNULL(format)
	if(format == pFormat){
		return;
	}
	
	DestroyTexture();
	pFormat = format;
}

void deoglTexture::SetFormatMappingByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber){
	SetFormat(&pRenderThread.GetCapabilities().GetFormats().RequireUseTex2DFormatFor(formatNumber));
}

void deoglTexture::SetFormatFBOByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber){
	SetFormat(&pRenderThread.GetCapabilities().GetFormats().RequireUseFBOTex2DFormatFor(formatNumber));
}

#elif defined BACKEND_VULKAN

void deoglTexture::SetFormat(const devkFormat *format){
	DEASSERT_NOTNULL(format)
	if(format == pFormat){
		return;
	}
	
	DestroyTexture();
	pFormat = format;
}

void deoglTexture::SetFormatMappingByNumber(devkDevice::eFormats format){
	SetFormat(pRenderThread.GetContext().GetDevice().GetUseTexFormat(format));
}

void deoglTexture::SetFormatFBOByNumber(devkDevice::eFormats format){
	SetFormat(pRenderThread.GetContext().GetDevice().GetUseFboFormat(format));
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
#ifdef BACKEND_OPENGL
	if(pTexture){
		return;
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const GLenum glpixelformat = pFormat->GetPixelFormat();
	const GLenum glpixeltype = pFormat->GetPixelType();
	const GLenum glformat = pFormat->GetFormat();
	
	OGL_CHECK(pRenderThread, glGenTextures(1, &pTexture));
	
	if(! pTexture){
		OGL_CHECK(pRenderThread, glGenTextures(1, &pTexture));
		DEASSERT_NOTNULL(pTexture)
	}
	
	tsmgr.EnableBareTexture(0, *this);
	
	if(pglTexStorage2D){
		if(pMipMapped){
			pRealMipMapLevelCount = pMipMapLevelCount;
			if(pRealMipMapLevelCount == 0){
				pRealMipMapLevelCount = (int)(floorf(log2f((float)decMath::max(pSize.x, pSize.y))));
			}
		}
		
		OGL_CHECK(pRenderThread, pglTexStorage2D(GL_TEXTURE_2D,
			pRealMipMapLevelCount + 1, glformat, pSize.x, pSize.y));
		
		/*
		GLint pvalue;
		OGL_CHECK(pRenderThread, glGetTexParameteriv(GL_TEXTURE_2D,
			GL_TEXTURE_IMMUTABLE_FORMAT, &pvalue));
		DEASSERT_TRUE(pvalue == GL_TRUE)
		*/
		
	}else{
		OGL_CHECK(pRenderThread, glTexImage2D(GL_TEXTURE_2D, 0, glformat,
			pSize.x, pSize.y, 0, glpixelformat, glpixeltype, NULL));
		
		if(pMipMapped){
			pRealMipMapLevelCount = pMipMapLevelCount;
			decPoint size(pSize);
			int i;
			
			if(pRealMipMapLevelCount == 0){
				pRealMipMapLevelCount = (int)(floorf(log2f((float)decMath::max(size.x, size.y))));
			}
			
			for(i=0; i<pRealMipMapLevelCount; i++){
				size = decPoint(size.x >> 1, size.y >> 1).Largest(decPoint(1, 1));
				OGL_CHECK(pRenderThread, glTexImage2D(GL_TEXTURE_2D, i + 1,
					glformat, size.x, size.y, 0, glpixelformat, glpixeltype, NULL));
			}
		}
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
	
#elif defined BACKEND_VULKAN
	if(pImage){
		return;
	}
	
	devkImageConfiguration config;
	config.Set2D(pSize, pFormat->GetFormat());
	
	if(pFormat->GetIsDepth() || pFormat->GetIsDepthFloat() || pFormat->GetIsStencil()){
		config.EnableDepthStencilAttachment(true);
		
	}else{
		config.EnableColorAttachment(true);
		config.EnableTransferSource(true);
		config.EnableTransferDestination(true);
	}
	
	if(pMipMapped){
		pRealMipMapLevelCount = pMipMapLevelCount;
		decPoint size(pSize);
		
		if(pRealMipMapLevelCount == 0){
			pRealMipMapLevelCount = (int)(floorf(log2f((float)decMath::max(size.x, size.y))));
		}
		
		config.SetMipMapCount(pRealMipMapLevelCount);
	}
	
	pImage.TakeOver(new devkImage(pRenderThread.GetContext().GetDevice(), config));
#endif
	
	UpdateMemoryUsage();
#ifdef BACKEND_OPENGL
	pUpdateDebugObjectLabel();
#endif
}

void deoglTexture::DestroyTexture(){
#ifdef BACKEND_OPENGL
	if(pTexture){
		pRenderThread.GetDelayedOperations().DeleteOpenGLTexture(pTexture);
		pTexture = 0;
		
		UpdateMemoryUsage();
	}
	
#elif defined BACKEND_VULKAN
	if(pImage){
		pImage = nullptr;
		pFormat = nullptr;
		
		UpdateMemoryUsage();
	}
#endif
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
	
	CreateTexture();
	
	const char * const pixelBufferData = (const char *)pixelBuffer.GetPointer()
		+ pixelBuffer.GetLayerStride() * layer;
	
#ifdef BACKEND_OPENGL
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	tsmgr.EnableBareTexture(0, *this);
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	
	if(pixelBuffer.GetCompressed()){
		// immutable textures allow only glCompressedTexSubImage2D
		//OGL_CHECK(pRenderThread, pglCompressedTexImage2D(GL_TEXTURE_2D, level,
		//	pFormat->GetFormat(), width, height, 0, pixelBuffer.GetImageSize(),
		//	(const GLvoid *)pixelBufferData));
		OGL_CHECK(pRenderThread, pglCompressedTexSubImage2D(GL_TEXTURE_2D, level,
			0, 0, width, height, pFormat->GetFormat(), pixelBuffer.GetImageSize(),
			(const GLvoid*)pixelBufferData));
		
	}else{
#ifdef OS_ANDROID
		oglClearError();
		pglTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, pixelBuffer.GetGLPixelFormat(),
			pixelBuffer.GetGLPixelType(), (const GLvoid *)pixelBufferData);
		if(glGetError() == GL_INVALID_OPERATION){
			pRenderThread.GetLogger().LogInfoFormat(
				"deoglTexture::SetPixelsLevelLayer Failed: size(%d,%d) level=%d f=%s pf=0x%x pt=0x%x",
				width, height, level, pFormat->GetName().GetString(),
				pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType());
			DETHROW(deeInvalidParam);
		}
#else
		OGL_CHECK(pRenderThread, pglTexSubImage2D(GL_TEXTURE_2D, level, 0, 0,
			width, height, pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType(),
			(const GLvoid*)pixelBufferData));
#endif
	}
	
	OGL_CHECK(pRenderThread, glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	
	tsmgr.DisableStage(0);
	
#elif defined BACKEND_VULKAN
	pImage->SetData(pixelBufferData, pixelBuffer.GetLayerStride() * layer,
		pixelBuffer.GetLayerStride());
#endif
}

void deoglTexture::GetPixels(deoglPixelBuffer &pixelBuffer) const{
	GetPixelsLevel(0, pixelBuffer);
}

void deoglTexture::GetPixelsLevel(int level, deoglPixelBuffer &pixelBuffer) const{
	int width, height;
	GetLevelSize(level, width, height);
	
#ifdef BACKEND_OPENGL
	/*if(pixelBuffer.GetWidth() != width || pixelBuffer.GetHeight() != height || pixelBuffer.GetDepth() != 1){
		pRenderThread.GetLogger().LogInfoFormat("PROBLEM! level=%d width(%d=%d) height(%d=%d) depth(%d)",
			level, pixelBuffer.GetWidth(), width, pixelBuffer.GetHeight(), height, pixelBuffer.GetDepth());
		DETHROW(deeInvalidParam);
	}*/
	if(!pTexture){
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

#elif defined BACKEND_VULKAN
	if(!pImage){
		return;
	}
	
	pImage->GetData(pixelBuffer.GetPointer(), 0, pixelBuffer.GetLayerStride());
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
	
#ifdef BACKEND_OPENGL
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	tsmgr.EnableBareTexture(0, *this);
	pglGenerateMipmap(GL_TEXTURE_2D);
	tsmgr.DisableStage(0);
	
#elif defined BACKEND_VULKAN
	devkCommandBuffer &cbuf = pImage->BeginCommandBuffer(
		pRenderThread.GetContext().GetCommandPoolGraphic());
	pImage->GenerateMipMaps(cbuf);
	cbuf.Submit();
#endif
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
	
#ifdef BACKEND_OPENGL
	if(withMipMaps && texture.GetMipMapped() && pMipMapped){
		const int srcMipMapLevelCount = texture.GetRealMipMapLevelCount();
		int i, mipMapLevelCount;
		
		mipMapLevelCount = pRealMipMapLevelCount;
		if(srcMipMapLevelCount < mipMapLevelCount){
			mipMapLevelCount = srcMipMapLevelCount;
		}
		
		for(i=0; i<=mipMapLevelCount; i++){
			if(pglCopyImageSubData){
				pglCopyImageSubData(texture.GetTexture(), GL_TEXTURE_2D, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_2D, i, destX, destY, 0, width, height, 1);
				
			}else if(pglCopyImageSubDataNV){
				pglCopyImageSubDataNV(texture.GetTexture(), GL_TEXTURE_2D, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_2D, i, destX, destY, 0, width, height, 1);
				
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
			pglCopyImageSubData(texture.GetTexture(), GL_TEXTURE_2D, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_2D, 0, destX, destY, 0, width, height, 1);
			
		}else if(pglCopyImageSubDataNV){
			pglCopyImageSubDataNV(texture.GetTexture(), GL_TEXTURE_2D, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_2D, 0, destX, destY, 0, width, height, 1);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
#elif defined BACKEND_VULKAN
	if(withMipMaps && texture.GetMipMapped() && pMipMapped){
		
	}else{
		
	}
#endif
}



void deoglTexture::UpdateMemoryUsage(){
	pMemUse.Clear();
	
#ifdef BACKEND_OPENGL
	if(! pTexture || ! pFormat){
		return;
	}
	
#ifdef OS_ANDROID
	pMemUse.SetUncompressed(*pFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
	
#else
	if(pFormat->GetIsCompressed()){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareTexture(0, *this);
		
		GLint isReallyCompressed = 0;
		OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_2D,
			0, GL_TEXTURE_COMPRESSED, &isReallyCompressed));
		
		if(isReallyCompressed){
			unsigned long consumption = 0ull;
			GLint compressedSize, l;
			for(l=0; l<=pRealMipMapLevelCount; l++){
				OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_2D, l,
					GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize));
				consumption += (unsigned long long)compressedSize;
			}
			
			pMemUse.SetCompressed(consumption, *pFormat);
			
		}else{
			pMemUse.SetUncompressed(*pFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
		}
		
		tsmgr.DisableStage(0);
		
	}else{
		pMemUse.SetUncompressed(*pFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
	}
#endif
	
#elif defined BACKEND_VULKAN
	if(!pImage || !pFormat){
		return;
	}
	
	if(pFormat->GetIsCompressed()){
		/*
		GLint isReallyCompressed = 0;
		OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_2D,
			0, GL_TEXTURE_COMPRESSED, &isReallyCompressed));
		
		if(isReallyCompressed){
			unsigned long consumption = 0ull;
			GLint compressedSize, l;
			for(l=0; l<=pRealMipMapLevelCount; l++){
				OGL_CHECK(pRenderThread, glGetTexLevelParameteriv(GL_TEXTURE_2D, l,
					GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize));
				consumption += (unsigned long long)compressedSize;
			}
			
			pMemUse.SetCompressed(consumption, *pFormat);
			
		}else{
			pMemUse.SetUncompressed(*pFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
		}
		*/
		
	}else{
		pMemUse.SetUncompressed(*pFormat, pSize.x, pSize.y, 1, pRealMipMapLevelCount);
	}
#endif
}



// Helper Functions
/////////////////////

#ifdef BACKEND_OPENGL
	#define HELPER_SET_FORMAT(funcType,constSuffix) \
		SetFormat ## funcType ## ByNumber(deoglCapsFmtSupport::eutf ## constSuffix);
#elif defined BACKEND_VULKAN
	#define HELPER_SET_FORMAT(funcType,constSuffix) \
		SetFormat ## funcType ## ByNumber(devkDevice::ef ## constSuffix);
#endif

void deoglTexture::SetMapingFormat(int channels, bool useFloat, bool compressed){
	if(channels == 1){
		if(useFloat){
			HELPER_SET_FORMAT(Mapping, R16F)
			
		}else{
			if(compressed){
				HELPER_SET_FORMAT(Mapping, R8_C)
				
			}else{
				HELPER_SET_FORMAT(Mapping, R8)
			}
		}
		
	}else if(channels == 2){
		if(useFloat){
			HELPER_SET_FORMAT(Mapping, RG16F)
			
		}else{
			if(compressed){
				HELPER_SET_FORMAT(Mapping, RG8_C)
				
			}else{
				HELPER_SET_FORMAT(Mapping, RG8)
			}
		}
		
	}else if(channels == 3){
		if(useFloat){
			HELPER_SET_FORMAT(Mapping, RGB16F)
			
		}else{
			if(compressed){
				HELPER_SET_FORMAT(Mapping, RGB8_C)
				
			}else{
				HELPER_SET_FORMAT(Mapping, RGB8)
			}
		}
		
	}else if(channels == 4){
		if(useFloat){
			HELPER_SET_FORMAT(Mapping, RGBA16F)
			
		}else{
			if(compressed){
				HELPER_SET_FORMAT(Mapping, RGBA8_C)
				
			}else{
				HELPER_SET_FORMAT(Mapping, RGBA8)
			}
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglTexture::SetFBOFormat(int channels, bool useFloat){
	if(channels == 1){
		if(useFloat){
			HELPER_SET_FORMAT(FBO, R16F)
			
		}else{
			HELPER_SET_FORMAT(FBO, R8)
		}
		
	}else if(channels == 2){
		if(useFloat){
			HELPER_SET_FORMAT(FBO, RG16F)
			
		}else{
			HELPER_SET_FORMAT(FBO, RG8)
		}
		
	}else if(channels == 3){
		if(useFloat){
			HELPER_SET_FORMAT(FBO, RGB16F)
			
		}else{
			HELPER_SET_FORMAT(FBO, RGB8)
		}
		
	}else if(channels == 4){
		if(useFloat){
			HELPER_SET_FORMAT(FBO, RGBA16F)
			
		}else{
			HELPER_SET_FORMAT(FBO, RGBA8)
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglTexture::SetFBOFormatFloat32(int channels){
	if(channels == 1){
		HELPER_SET_FORMAT(FBO, R32F)
		
	}else if(channels == 2){
		HELPER_SET_FORMAT(FBO, RG32F)
		
	}else if(channels == 3){
		HELPER_SET_FORMAT(FBO, RGB32F)
		
	}else if(channels == 4){
		HELPER_SET_FORMAT(FBO, RGBA32F)
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglTexture::SetFBOFormatIntegral(int channels, int bpp, bool useUnsigned){
	if(bpp == 8){
		if(channels == 1){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, R8UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, R8I)
			}
			
		}else if(channels == 2){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, RG8UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, RG8I)
			}
			
		}else if(channels == 3){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, RGB8UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, RGB8I)
			}
			
		}else if(channels == 4){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, RGBA8UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, RGBA8I)
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else if(bpp == 16){
		if(channels == 1){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, R16UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, R16I)
			}
			
		}else if(channels == 2){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, RG16UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, RG16I)
			}
			
		}else if(channels == 3){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, RGB16UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, RGB16I)
			}
			
		}else if(channels == 4){
			if(useUnsigned){
				HELPER_SET_FORMAT(FBO, RGBA16UI)
				
			}else{
				HELPER_SET_FORMAT(FBO, RGBA16I)
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deoglTexture::SetFBOFormatSNorm(int channels, int bpp){
	switch(bpp){
	case 8:
		switch(channels){
		case 1:
			HELPER_SET_FORMAT(FBO, R8_S)
			break;
			
		case 2:
			HELPER_SET_FORMAT(FBO, RG8_S)
			break;
			
		case 3:
			HELPER_SET_FORMAT(FBO, RGB8_S)
			break;
			
		case 4:
			HELPER_SET_FORMAT(FBO, RGBA8_S)
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		break;
		
	case 16:
		switch(channels){
		case 1:
			HELPER_SET_FORMAT(FBO, R16_S)
			break;
			
		case 2:
			HELPER_SET_FORMAT(FBO, RG16_S)
			break;
			
		case 3:
			HELPER_SET_FORMAT(FBO, RGB16_S)
			break;
			
		case 4:
			HELPER_SET_FORMAT(FBO, RGBA16_S)
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void deoglTexture::SetDepthFormat(bool packedStencil, bool useFloat){
	if(packedStencil){
		if(useFloat){
			HELPER_SET_FORMAT(FBO, DepthF_Stencil)
			
		}else{
			HELPER_SET_FORMAT(FBO, Depth_Stencil)
		}
		
	}else{
		if(useFloat){
			HELPER_SET_FORMAT(FBO, DepthF)
			
		}else{
			HELPER_SET_FORMAT(FBO, Depth)
		}
	}
}

#ifdef BACKEND_OPENGL
void deoglTexture::SetDebugObjectLabel(const char *name){
	pDebugObjectLabel.Format("2D: %s", name);
	if(pTexture){
		pUpdateDebugObjectLabel();
	}
}
#endif


// Private Functions
//////////////////////

#ifdef BACKEND_OPENGL
void deoglTexture::pUpdateDebugObjectLabel(){
	pRenderThread.GetDebug().SetDebugObjectLabel(GL_TEXTURE, pTexture, pDebugObjectLabel);
}
#endif
