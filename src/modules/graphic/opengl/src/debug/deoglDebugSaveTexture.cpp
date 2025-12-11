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
#include <string.h>
#include <stdlib.h>

#include "deoglDebugSaveTexture.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../delayedoperation/deoglDelayedSaveImage.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Definitions
////////////////

#define CONVERT_COLOR_LIN2SRGB   (1.0f / 2.2f)



// Class deoglDebugSaveTexture
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugSaveTexture::deoglDebugSaveTexture(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
}

deoglDebugSaveTexture::~deoglDebugSaveTexture(){
}



// Management
///////////////

void deoglDebugSaveTexture::SetBasePath(const char *path){
	if(!path) DETHROW(deeInvalidParam);
	
	pBasePath = path;
}

void deoglDebugSaveTexture::SaveTexture(deoglTexture &texture, const char *name){
	SaveTextureLevelConversion(texture, 0, name, ecNoConversion);
}

void deoglDebugSaveTexture::SaveTextureConversion(deoglTexture &texture, const char *name, eConvertions conversion){
	SaveTextureLevelConversion(texture, 0, name, conversion);
}

void deoglDebugSaveTexture::SaveTextureLevel(deoglTexture &texture, int level, const char *name){
	SaveTextureLevelConversion(texture, level, name, ecNoConversion);
}

void deoglDebugSaveTexture::SaveTextureLevelConversion(deoglTexture &texture, int level, const char *name, eConvertions conversion){
	if(level < 0 || !name){
		DETHROW(deeInvalidParam);
	}
	
	deoglDelayedSaveImage *saveImage = NULL;
	int height = texture.GetHeight();
	int width = texture.GetWidth();
	decString fileTitle;
	decPath pathFile;
	sRGBA8 *imgdata = NULL;
	int i;
	
	for(i=0; i<level; i++){
		width >>= 1;
		if(width < 1){
			width = 1;
		}
		
		height >>= 1;
		if(height < 1){
			height = 1;
		}
	}
	
	fileTitle.Format("%s.png", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		const deoglPixelBuffer::Ref pixbuf(deoglPixelBuffer::Ref::NewWith(
			deoglPixelBuffer::epfFloat4, width, height, 1));
		
		texture.GetPixelsLevel(level, pixbuf);
		
		imgdata = new sRGBA8[width * height];
		pConvertDataRGBA(pixbuf->GetPointerFloat4(), imgdata, width, height, false, conversion);
		
		saveImage = new deoglDelayedSaveImage(pathFile, width, height, 1, 4, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
	}catch(const deException &e){
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveTextureLevel(%s:%i) failed!", name, level);
		e.PrintError();
	}
}

void deoglDebugSaveTexture::SaveDepthTexture(deoglTexture &texture, const char *name, eDepthTypes type){
	SaveDepthTextureLevel(texture, 0, name, type);
}

void deoglDebugSaveTexture::SaveDepthTextureLevel(deoglTexture &texture, int level, const char *name, eDepthTypes type){
	if(level < 0 || !name){
		DETHROW(deeInvalidParam);
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	deoglFramebuffer * const oldFBO = pRenderThread.GetFramebuffer().GetActive();
	deoglDelayedSaveImage *saveImage = NULL;
	int height = texture.GetHeight();
	int width = texture.GetWidth();
	deoglFramebuffer::Ref fbo;
	decString fileTitle;
	decPath pathFile;
	int y, x;
	sRGBA8 *imgdata = NULL;
	float depthval;
	
	for(y=0; y<level; y++){
		width >>= 1;
		if(width < 1){
			width = 1;
		}
		
		height >>= 1;
		if(height < 1){
			height = 1;
		}
	}
	
	fileTitle.Format("%s.png", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		const deoglPixelBuffer::Ref pixbuf(deoglPixelBuffer::Ref::NewWith(
			deoglPixelBuffer::epfFloat1, width, height, 1));
		deoglPixelBuffer::sFloat1 * const pbdata = pixbuf->GetPointerFloat1();
		
		if(true){
			fbo.TakeOverWith(pRenderThread, false);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachDepthTextureLevel(&texture, level);
			const GLenum buffers[1] = {GL_NONE};
			OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
			OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
			fbo->Verify();
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
			OGL_CHECK(pRenderThread, glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, pbdata));
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
			
		}else{
			tsmgr.EnableBareTexture(0, texture);
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
			OGL_CHECK(pRenderThread, glGetTexImage(GL_TEXTURE_2D, level, GL_DEPTH_COMPONENT, GL_FLOAT, pbdata));
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
			tsmgr.DisableStage(0);
		}
		
		imgdata = new sRGBA8[width * height];
		
		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				sRGBA8 &d = imgdata[(height - 1 - y) * width + x];
				const deoglPixelBuffer::sFloat1 &s = pbdata[y * width + x];
				
				switch(type){
				case edtLinear:
					depthval = s.r;
					break;
					
				case edtDepth:
					depthval = powf(s.r, 250.0f);
					break;
					
				case edtLinearInverse:
					depthval = 1.0f - s.r;
					break;
					
				case edtDepthInverse:
					depthval = powf(1.0f - s.r, 250.0f);
					break;
					
				default:
					depthval = 0.0f;
				}
				
				d.red = (unsigned char)(depthval * 255.0f);
				d.green = d.red;
				d.blue = d.red;
				d.alpha = 255;
			}
		}
		
		saveImage = new deoglDelayedSaveImage(pathFile, width, height, 1, 4, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
		pRenderThread.GetFramebuffer().Activate(oldFBO);
		
	}catch(const deException &e){
		pRenderThread.GetFramebuffer().Activate(oldFBO);
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveDepthTextureLevel(%s) failed!", name);
		e.PrintError();
	}
}

void deoglDebugSaveTexture::SaveStencilTexture(deoglTexture &texture, const char *name){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	deoglFramebuffer * const oldFBO = pRenderThread.GetFramebuffer().GetActive();
	deoglDelayedSaveImage *saveImage = NULL;
	int height = texture.GetHeight();
	int width = texture.GetWidth();
	deoglFramebuffer::Ref fbo;
	decString fileTitle;
	decPath pathFile;
	int y, x;
	sRGB8 *imgdata = NULL;
	unsigned char *pbdata = NULL;
	
	fileTitle.Format("%s.png", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		pbdata = new unsigned char[width * height];
		imgdata = new sRGB8[width * height];
		
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
		
		if(true){
			fbo.TakeOverWith(pRenderThread, false);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachDepthTextureLevel(&texture, 0);
			fbo->AttachStencilTextureLevel(&texture, 0);
			const GLenum buffers[1] = {GL_NONE};
			OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
			OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
			fbo->Verify();
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
			OGL_CHECK(pRenderThread, glReadPixels(0, 0, width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pbdata));
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
			
		}else{
			tsmgr.EnableBareTexture(0, texture);
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
			OGL_CHECK(pRenderThread, glGetTexImage(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pbdata));
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
			tsmgr.DisableStage(0);
		}
		
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
		
		for(y=0; y<height; y++){
			for(x=0; x<width; x++){
				sRGB8 &d = imgdata[(height - 1 - y) * width + x];
				const unsigned char s = pbdata[y * width + x];
				
				d.red = s; //(unsigned char)(s & 0xff);
				d.green = s;
				d.blue = s;
			}
		}
		
		saveImage = new deoglDelayedSaveImage(pathFile, width, height, 1, 3, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
		delete [] pbdata;
		pbdata = NULL;
		
		pRenderThread.GetFramebuffer().Activate(oldFBO);
		
	}catch(const deException &e){
		pRenderThread.GetFramebuffer().Activate(oldFBO);
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		if(pbdata){
			delete [] pbdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveStencilTexture(%s) failed!", name);
		e.PrintError();
	}
}

void deoglDebugSaveTexture::SaveStencilArrayTexture(deoglArrayTexture &texture, const char *name){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	deoglFramebuffer * const oldFBO = pRenderThread.GetFramebuffer().GetActive();
	deoglDelayedSaveImage *saveImage = NULL;
	int height = texture.GetHeight();
	int width = texture.GetWidth();
	int layers = texture.GetLayerCount();
	deoglFramebuffer::Ref fbo;
	decString fileTitle;
	decPath pathFile;
	int y, x, l;
	sRGB8 *imgdata = NULL;
	unsigned char *pbdata = NULL;
	
	fileTitle.Format("%s.png3d", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		pbdata = new unsigned char[width * height * layers];
		imgdata = new sRGB8[width * height * layers];
		
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
		
		if(true){
			fbo.TakeOverWith(pRenderThread, false);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachDepthArrayTextureLevel(&texture, 0);
			fbo->AttachStencilArrayTextureLevel(&texture, 0);
			const GLenum buffers[1] = {GL_NONE};
			OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
			OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
			fbo->Verify();
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
			OGL_CHECK(pRenderThread, glReadPixels(0, 0, width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pbdata));
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
			
		}else{
			tsmgr.EnableBareArrayTexture(0, texture);
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 1));
			OGL_CHECK(pRenderThread, glGetTexImage(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pbdata));
			OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
			tsmgr.DisableStage(0);
		}
		
		OGL_CHECK(pRenderThread, glPixelStorei(GL_PACK_ALIGNMENT, 4));
		
		for(l=0; l<layers; l++){
			for(y=0; y<height; y++){
				for(x=0; x<width; x++){
					sRGB8 &d = imgdata[(width * height) * l + (height - 1 - y) * width + x];
					const unsigned char s = pbdata[y * width + x];
					
					d.red = s; //(unsigned char)(s & 0xff);
					d.green = s;
					d.blue = s;
				}
			}
		}
		
		saveImage = new deoglDelayedSaveImage(pathFile, width, height, layers, 3, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
		delete [] pbdata;
		pbdata = NULL;
		
		pRenderThread.GetFramebuffer().Activate(oldFBO);
		
	}catch(const deException &e){
		pRenderThread.GetFramebuffer().Activate(oldFBO);
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		if(pbdata){
			delete [] pbdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveStencilTexture(%s) failed!", name);
		e.PrintError();
	}
}



void deoglDebugSaveTexture::SaveCubeMap(deoglCubeMap &cubemap, const char *name, bool upsideDown){
	SaveCubeMapLevelConversion(cubemap, 0, name, upsideDown, ecNoConversion);
}

void deoglDebugSaveTexture::SaveCubeMapLevel(deoglCubeMap &cubemap, int level, const char *name, bool upsideDown){
	SaveCubeMapLevelConversion(cubemap, level, name, upsideDown, ecNoConversion);
}

void deoglDebugSaveTexture::SaveCubeMapConversion(deoglCubeMap &cubemap, const char *name, bool upsideDown, eConvertions conversion){
	SaveCubeMapLevelConversion(cubemap, 0, name, upsideDown, conversion);
}

void deoglDebugSaveTexture::SaveCubeMapLevelConversion(deoglCubeMap &cubemap, int level, const char *name, bool upsideDown, eConvertions conversion){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int size = cubemap.GetLevelSize(level);
	deoglDelayedSaveImage *saveImage = NULL;
	decString fileTitle;
	decPath pathFile;
	sRGBA8 *imgdata = NULL;
	int i;
	
	fileTitle.Format("%s.png3d", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		const deoglPixelBuffer::Ref pixbuf(deoglPixelBuffer::Ref::NewWith(
			deoglPixelBuffer::epfFloat4, size, size, 6));
		deoglPixelBuffer::sFloat4 * const pbdata = pixbuf->GetPointerFloat4();
		const int faceStride = size * size;
		
		imgdata = new sRGBA8[faceStride * 6];
		
		cubemap.GetPixelsLevel(level, pixbuf);
		
		for(i=0; i<6; i++){
			pConvertDataRGBA(pbdata + faceStride * i, imgdata + faceStride * i, size, size, upsideDown, conversion);
		}
		
		saveImage = new deoglDelayedSaveImage(pathFile, size, size, 6, 4, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
	}catch(const deException &e){
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveCubeMap(%s) failed!", name);
		e.PrintError();
	}
}


void deoglDebugSaveTexture::SaveDepthCubeMap(deoglCubeMap &cubemap, const char *name, bool linearDepth){
	SaveDepthCubeMapLevel(cubemap, 0, name, linearDepth);
}

void deoglDebugSaveTexture::SaveDepthCubeMapLevel(deoglCubeMap &cubemap, int level, const char *name, bool linearDepth){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int size = cubemap.GetLevelSize(level);
	deoglDelayedSaveImage *saveImage = NULL;
	decString fileTitle;
	decPath pathFile;
	sRGBA8 *imgdata = NULL;
	float depthval;
	int cmf, x, y;
	
	fileTitle.Format("%s.png3d", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		const deoglPixelBuffer::Ref pixbuf(deoglPixelBuffer::Ref::NewWith(
			deoglPixelBuffer::epfFloat1, size, size, 6));
		const deoglPixelBuffer::sFloat1 * const pbdata = pixbuf->GetPointerFloat1();
		const int faceStride = size * size;
		
		imgdata = new sRGBA8[faceStride * 6];
		
		cubemap.GetPixelsLevel(level, pixbuf);
		
		for(cmf=0; cmf<6; cmf++){
			const deoglPixelBuffer::sFloat1 * const pbdface = pbdata + faceStride * cmf;
			
			for(y=0; y<size; y++){
				for(x=0; x<size; x++){
					sRGBA8 &d = imgdata[faceStride * cmf + size * (size - 1 - y) + x];
					const deoglPixelBuffer::sFloat1 &s = pbdface[size * y + x];
					
					if(linearDepth){
						depthval = s.r;
						
					}else{
						depthval = powf(s.r, 200.0f);
					}
					
					d.red = (unsigned char)(depthval * 255.0f);
					d.green = d.red;
					d.blue = d.red;
					d.alpha = 255;
				}
			}
		}
		
		saveImage = new deoglDelayedSaveImage(pathFile, size, size, 6, 4, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
	}catch(const deException &e){
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveDepthCubeMap(%s) failed!", name);
		e.PrintError();
	}
}

void deoglDebugSaveTexture::SaveArrayTexture(deoglArrayTexture &texture, const char *name){
	SaveArrayTextureLevelConversion(texture, 0, name, ecNoConversion);
}

void deoglDebugSaveTexture::SaveArrayTextureLevel(deoglArrayTexture &texture, int level, const char *name){
	SaveArrayTextureLevelConversion(texture, level, name, ecNoConversion);
}

void deoglDebugSaveTexture::SaveArrayTextureConversion(deoglArrayTexture &texture,
const char *name, eConvertions conversion){
	SaveArrayTextureLevelConversion(texture, 0, name, conversion);
}

void deoglDebugSaveTexture::SaveArrayTextureLevelConversion(deoglArrayTexture &texture,
int level, const char *name, eConvertions conversion){
	if(level < 0 || !name){
		DETHROW(deeInvalidParam);
	}
	
	deoglDelayedSaveImage *saveImage = NULL;
	const int layerCount = texture.GetLayerCount();
	int height, width;
	decString fileTitle;
	decPath pathFile;
	sRGBA8 *imgdata = NULL;
	int i;
	
	texture.GetLevelSize(level, width, height);
	
	fileTitle.Format("%s.png3d", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		const deoglPixelBuffer::Ref pixbuf(deoglPixelBuffer::Ref::NewWith(
			deoglPixelBuffer::epfFloat4, width, height, layerCount));
		const deoglPixelBuffer::sFloat4 * const pbdata = pixbuf->GetPointerFloat4();
		const int stride = width * height;
		
		texture.GetPixelsLevel(level, pixbuf);
		
		imgdata = new sRGBA8[stride * layerCount];
		
		for(i=0; i<layerCount; i++){
			pConvertDataRGBA(pbdata + stride * i, imgdata, width, height, true, conversion);
		}
		
		saveImage = new deoglDelayedSaveImage(pathFile, width, height, layerCount, 4, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
	}catch(const deException &e){
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveArrayTextureLevelConversion(%s:%i) failed!", name, level);
		e.PrintError();
	}
}

void deoglDebugSaveTexture::SaveDepthArrayTexture(deoglArrayTexture &arrayTexture, const char *name, bool linearDepth){
	SaveDepthArrayTextureLevel(arrayTexture, 0, name, linearDepth);
}

void deoglDebugSaveTexture::SaveDepthArrayTextureLevel(deoglArrayTexture &arrayTexture, int level, const char *name, bool linearDepth){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	deoglDelayedSaveImage *saveImage = NULL;
	int layerCount = arrayTexture.GetLayerCount();
	int width, height;
	decString fileTitle;
	decPath pathFile;
	int l, x, y;
	sRGBA8 *imgdata = NULL;
	float depthval;
	
	arrayTexture.GetLevelSize(level, width, height);
	
	fileTitle.Format("%s.png3d", name);
	pathFile.SetFromUnix(pBasePath.GetString());
	pathFile.AddUnixPath(fileTitle.GetString());
	
	try{
		const deoglPixelBuffer::Ref pixbuf(deoglPixelBuffer::Ref::NewWith(
			deoglPixelBuffer::epfFloat1, width, height, layerCount));
		const deoglPixelBuffer::sFloat1 * const pbdata = pixbuf->GetPointerFloat1();
		
		arrayTexture.GetPixelsLevel(level, pixbuf);
		
		imgdata = new sRGBA8[width * height * layerCount];
		
		for(l=0; l<layerCount; l++){
			for(y=0; y<height; y++){
				for(x=0; x<width; x++){
					sRGBA8 &d = imgdata[width * height * l + width * (height - 1 - y) + x];
					const deoglPixelBuffer::sFloat1 &s = pbdata[width * height * l + width * y + x];
					
					if(linearDepth){
						depthval = s.r;
						
					}else{
						depthval = powf(s.r, 200.0f);
					}
					
					d.red = (unsigned char)(depthval * 255.0f);
					d.green = d.red;
					d.blue = d.red;
					d.alpha = 255;
				}
			}
		}
		
		saveImage = new deoglDelayedSaveImage(pathFile, width, height, layerCount, 4, 8, (char*)imgdata);
		imgdata = NULL;
		
		pRenderThread.GetDelayedOperations().AddSaveImage(saveImage);
		saveImage = NULL;
		
	}catch(const deException &e){
		if(saveImage){
			delete saveImage;
		}
		if(imgdata){
			delete [] imgdata;
		}
		
		pRenderThread.GetLogger().LogErrorFormat("DebugSaveTexture.SaveDepthArrayTexture(%s) failed!", name);
		e.PrintError();
	}
}



deoglPixelBuffer::ePixelFormats deoglDebugSaveTexture::GetPixelBufferType(GLenum &pixelFormat, GLenum &pixelType) const{
	// byte and float formats
	if(pixelFormat == GL_LUMINANCE){
		pixelFormat = GL_RGBA;
		if(pixelType == GL_UNSIGNED_BYTE){
			return deoglPixelBuffer::epfByte4; //deoglPixelBuffer::epfByte1;
			
		}else if(pixelType == GL_FLOAT){
			pixelType = GL_UNSIGNED_BYTE;
			return deoglPixelBuffer::epfByte4; //deoglPixelBuffer::epfByte1;// deoglPixelBuffer::epfFloat1;
		}
		
	}else if(pixelFormat == GL_LUMINANCE_ALPHA){
		pixelFormat = GL_RGBA;
		if(pixelType == GL_UNSIGNED_BYTE){
			return deoglPixelBuffer::epfByte4; //deoglPixelBuffer::epfByte2;
			
		}else if(pixelType == GL_FLOAT){
			pixelType = GL_UNSIGNED_BYTE;
			return deoglPixelBuffer::epfByte4; //deoglPixelBuffer::epfByte2; //deoglPixelBuffer::epfFloat2;
		}
		
	}else if(pixelFormat == GL_RGB){
		pixelFormat = GL_RGBA;
		if(pixelType == GL_UNSIGNED_BYTE){
			return deoglPixelBuffer::epfByte4;
			
		}else if(pixelType == GL_FLOAT){
			pixelType = GL_UNSIGNED_BYTE;
			return deoglPixelBuffer::epfByte4; //deoglPixelBuffer::epfFloat3;
		}
		
	}else if(pixelFormat == GL_RGBA){
		pixelFormat = GL_RGBA;
		if(pixelType == GL_UNSIGNED_BYTE){
			return deoglPixelBuffer::epfByte4;
			
		}else if(pixelType == GL_FLOAT){
			pixelType = GL_UNSIGNED_BYTE;
			return deoglPixelBuffer::epfByte4; //deoglPixelBuffer::epfFloat4;
		}
		
	// integer formats
	}else if(pixelFormat == GL_RED_INTEGER){
		pixelFormat = GL_RGBA_INTEGER;
		return deoglPixelBuffer::epfByte4;
		
	}else if(pixelFormat == GL_RG_INTEGER){
		pixelFormat = GL_RGBA_INTEGER;
		return deoglPixelBuffer::epfByte4;
		
	}else if(pixelFormat == GL_RGB_INTEGER){
		pixelFormat = GL_RGBA_INTEGER;
		return deoglPixelBuffer::epfByte4;
		
	}else if(pixelFormat == GL_RGBA_INTEGER){
		pixelFormat = GL_RGBA_INTEGER;
		return deoglPixelBuffer::epfByte4;
		
	// depth formats
	}else if(pixelFormat == GL_DEPTH_COMPONENT){
		pixelType = GL_UNSIGNED_BYTE;
		return deoglPixelBuffer::epfByte1; //deoglPixelBuffer::epfFloat1;
	}
	
	DETHROW(deeInvalidParam);
}

int deoglDebugSaveTexture::GetStride(int width, int pixelBufferType) const{
	switch((deoglPixelBuffer::ePixelFormats)pixelBufferType){
	case deoglPixelBuffer::epfByte1:
		return width; // sizeof(char) * 1
		
	case deoglPixelBuffer::epfByte2:
		return width * 2; // sizeof(char) * 2
		
	case deoglPixelBuffer::epfByte3:
		return width * 3; // sizeof(char) * 3
		
	case deoglPixelBuffer::epfByte4:
		return width * 4; // sizeof(char) * 4
		
	case deoglPixelBuffer::epfFloat1:
		return width * 4; // sizeof(float) * 1
		
	case deoglPixelBuffer::epfFloat2:
		return width * 8; // sizeof(float) * 2
		
	case deoglPixelBuffer::epfFloat3:
		return width * 12; // sizeof(float) * 3
		
	case deoglPixelBuffer::epfFloat4:
		return width * 16; // sizeof(float) * 4
		
	default:
		DETHROW(deeInvalidParam);
	}
}

int deoglDebugSaveTexture::GetComponentCount(int pixelBufferType){
	switch((deoglPixelBuffer::ePixelFormats)pixelBufferType){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfFloat1:
		return 1;
		
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfFloat2:
		return 2;
		
	case deoglPixelBuffer::epfByte3:
	case deoglPixelBuffer::epfFloat3:
		return 3;
		
	case deoglPixelBuffer::epfByte4:
	case deoglPixelBuffer::epfFloat4:
		return 4;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

int deoglDebugSaveTexture::GetBitCount(int pixelBufferType){
	switch((deoglPixelBuffer::ePixelFormats)pixelBufferType){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfByte3:
	case deoglPixelBuffer::epfByte4:
		return 8;
		
	case deoglPixelBuffer::epfFloat1:
	case deoglPixelBuffer::epfFloat2:
	case deoglPixelBuffer::epfFloat3:
	case deoglPixelBuffer::epfFloat4:
		return 32;
		
	default:
		DETHROW(deeInvalidParam);
	}
}



// Private Functions
//////////////////////

void deoglDebugSaveTexture::pConvertDataRGBA(const deoglPixelBuffer::sFloat4 *inputData,
sRGBA8 *outputData, int width, int height, bool yflip, eConvertions conversion){
	const float rangeHigh = 6.0f;
	const float rangeLow = -7.0f;
	const float rangeInv = 1.0f / (rangeHigh - rangeLow);
	
	int srcy, srcyinc;
	deoglPixelBuffer::sFloat4 value;
	int x, y;
	
	if(yflip){
		srcy = 0;
		srcyinc = width;
		
	}else{
		srcy = (height - 1) * width;
		srcyinc = -width;
	}
	
	for(y=0; y<height; srcy+=srcyinc, y++){
		for(x=0; x<width; x++){
			sRGBA8 &d = outputData[srcy + x];
			const deoglPixelBuffer::sFloat4 &s = inputData[y * width + x];
			
			switch(conversion){
			case ecColorLinear2sRGB:
				value.r = powf(s.r, CONVERT_COLOR_LIN2SRGB);
				value.g = powf(s.g, CONVERT_COLOR_LIN2SRGB);
				value.b = powf(s.b, CONVERT_COLOR_LIN2SRGB);
				value.a = s.a;
				break;
				
			case ecColorLinearToneMapsRGB:
				value.r = powf(s.r / (1.0f + s.r), 1.0f/*CONVERT_COLOR_LIN2SRGB*/);
				value.g = powf(s.g / (1.0f + s.g), 1.0f/*CONVERT_COLOR_LIN2SRGB*/);
				value.b = powf(s.b / (1.0f + s.b), 1.0f/*CONVERT_COLOR_LIN2SRGB*/);
				value.a = s.a;
				break;
				
			case ecColorLinearScale10:
				value.r = s.r * 0.1f;
				value.g = s.g * 0.1f;
				value.b = s.b * 0.1f;
				value.a = s.a;
				break;
				
			case ecDepthBuffer:
				value.r = powf(s.r, 200.0f);
				value.g = powf(s.g, 200.0f);
				value.b = powf(s.b, 200.0f);
				value.a = s.a;
				break;
				
			case ecDepthBufferInverse:
				value.r = powf(1.0f - s.r, 300.0f);
				value.g = powf(1.0f - s.g, 300.0f);
				value.b = powf(1.0f - s.b, 300.0f);
				value.a = s.a;
				break;
				
			case ecNormal:
				value.r = s.r * 0.5f + 0.5f;
				value.g = s.g * 0.5f + 0.5f;
				value.b = s.b * 0.5f + 0.5f;
				value.a = sqrtf(s.r * s.r + s.g * s.g + s.b * s.b) * 0.5f;
				break;
				
			case ecNaNInf:
				value.r = ((isnan(s.r) || isnan(s.g) || isnan(s.b) || isnan(s.a))) ? 1.0f : 0.0f;
				value.g = ((isinf(s.r) || isinf(s.g) || isinf(s.b) || isinf(s.a))) ? 1.0f : 0.0f;
				value.b = 0.0f;
				value.a = 1.0f;
				break;
				
			case ecLogIntensity:
				value.r = (s.r - rangeLow) * rangeInv;
				value.g = (s.g - rangeLow) * rangeInv;
				value.b = (s.b - rangeLow) * rangeInv;
				value.a = (s.a - rangeLow) * rangeInv;
				break;
				
			default:
				value.r = s.r;
				value.g = s.g;
				value.b = s.b;
				value.a = s.a;
			}
			
			d.red = (unsigned char)(value.r * 255.0f);
			d.green = (unsigned char)(value.g * 255.0f);
			d.blue = (unsigned char)(value.b * 255.0f);
			d.alpha = (unsigned char)(value.a * 255.0f);
		}
	}
}
