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

#include <stdlib.h>
#include <string.h>

#include "deoxrSwapchain.h"
#include "deoxrSession.h"
#include "deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrSwapchain
/////////////////////////

deoxrSwapchain::deoxrSwapchain(deoxrSession &session, const decPoint &size, eType type) :
pSession(session),
pType(type),
pSize(size),
pSwapchain(XR_NULL_HANDLE),
pImages(nullptr),
pImageCount(0),
pVRRenderFormat(deBaseVRModule::evrrfRGB8)
{
	deoxrInstance &instance = session.GetSystem().GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	
	try{
		XrSwapchainCreateInfo createInfo{XR_TYPE_SWAPCHAIN_CREATE_INFO};
		createInfo.createFlags = 0;
		createInfo.usageFlags = XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT;
		createInfo.sampleCount = 1;
		createInfo.width = size.x;
		createInfo.height = size.y;
		createInfo.faceCount = 1;
		createInfo.arraySize = 1;
		createInfo.mipCount = 1;
		
		const int64_t * const formats = session.GetSwapchainFormats();
		const int formatCount = session.GetSwapchainFormatCount();
		int i;
		
		switch(session.GetGraphicApi()){
		case deoxrSession::egaOpenGL:
			switch(type){
			case etColor:
				for(i=0; !createInfo.format && i<formatCount; i++){
					switch(formats[i]){
#ifndef OS_ANDROID
					case deoxrSession::escfGlRgb8Snorm:
						pVRRenderFormat = deBaseVRModule::evrrfRGB8;
						createInfo.format = formats[i];
						break;
						
					case deoxrSession::escfGlRgb16:
					case deoxrSession::escfGlRgb16Snorm:
					case deoxrSession::escfGlRgb16f:
					case deoxrSession::escfGlRgb16i:
					case deoxrSession::escfGlRgb16ui:
					case deoxrSession::escfGlR11fG11fB10f:
						pVRRenderFormat = deBaseVRModule::evrrfRGB16;
						createInfo.format = formats[i];
						break;
						
					case deoxrSession::escfGlSrgb8:
						pVRRenderFormat = deBaseVRModule::evrrfSRGB8;
						createInfo.format = formats[i];
						break;
#endif
					case deoxrSession::escfGlRgba8:
					case deoxrSession::escfGlRgba8Snorm:
						pVRRenderFormat = deBaseVRModule::evrrfRGBA8;
						createInfo.format = formats[i];
						break;
						
					case deoxrSession::escfGlRgba16:
					case deoxrSession::escfGlRgba16Snorm:
					case deoxrSession::escfGlRgba16f:
					case deoxrSession::escfGlRgba16i:
					case deoxrSession::escfGlRgba16ui:
					case deoxrSession::escfGlRgb10a2:
						pVRRenderFormat = deBaseVRModule::evrrfRGBA16;
						createInfo.format = formats[i];
						break;
						
					case deoxrSession::escfGlSrgb8Alpha8:
						pVRRenderFormat = deBaseVRModule::evrrfSRGBA8;
						createInfo.format = formats[i];
						break;
						
					default:
						break;
					}
				}
				break;
				
			case etDepth:
				for(i=0; !createInfo.format && i<formatCount; i++){
					switch(formats[i]){
					case deoxrSession::escfGlDepth16:
					case deoxrSession::escfGlDepth24:
					case deoxrSession::escfGlDepth32F:
					case deoxrSession::escfGlDepth32Stencil8:
						oxr.LogInfoFormat("Using Depth Swapchain format %s",
							session.GetSwapchainFormatNameOpenGL(formats[i], "??"));
						createInfo.format = formats[i];
						break;
						
					default:
						break;
					}
				}
				break;
			}
			
			if(createInfo.format){
				oxr.LogInfoFormat("Using Color Swapchain format %s",
					session.GetSwapchainFormatNameOpenGL(createInfo.format, "??"));
			}
			break;
			
		default:
			DETHROW_INFO(deeInvalidParam, "no graphic api");
		}
		
		if(!createInfo.format){
			DETHROW_INFO(deeInvalidParam, "no supported Swapchain format found");
		}
		
		OXR_CHECK(instance.xrCreateSwapchain(session.GetSession(), &createInfo, &pSwapchain));
		
		pGetImages();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrSwapchain::~deoxrSwapchain(){
	pCleanUp();
}



// Management
///////////////

const deoxrSwapchain::sImage &deoxrSwapchain::GetImageAt(int index) const{
	if(index < 0 || index >= pImageCount){
		DETHROW(deeInvalidParam);
	}
	return pImages[index];
}

void deoxrSwapchain::AcquireImage(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	// acquire image
	OXR_CHECK(instance.xrAcquireSwapchainImage(pSwapchain, nullptr, &pAcquiredImage));
	
	// wait for image
	XrSwapchainImageWaitInfo waitInfo;
	memset(&waitInfo, 0, sizeof(waitInfo));
	waitInfo.type = XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO;
	waitInfo.timeout = XR_INFINITE_DURATION;
	
	OXR_CHECK(instance.xrWaitSwapchainImage(pSwapchain, &waitInfo));
	
	if(pSession.GetIsGACOpenGL()){
		// WARNING SteamVR messes with the current context state causing all future OpenGL
		//         calls to fail. not sure why SteamVR unsets the current context but it
		//         breaks everything. i dont know if the spec would actually requires
		//         runtimes to restore current context if they change it but to work
		//         around such annoying runtimes the current context is restored to the
		//         parameters provided by the graphic module
		pSession.RestoreOpenGLCurrent();
	}
}

void deoxrSwapchain::ReleaseImage(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	OXR_CHECK(instance.xrReleaseSwapchainImage(pSwapchain, nullptr));
	
	if(pSession.GetIsGACOpenGL()){
		// WARNING SteamVR messes with the current context state causing all future OpenGL
		//         calls to fail. not sure why SteamVR unsets the current context but it
		//         breaks everything. i dont know if the spec would actually requires
		//         runtimes to restore current context if they change it but to work
		//         around such annoying runtimes the current context is restored to the
		//         parameters provided by the graphic module
		pSession.RestoreOpenGLCurrent();
	}
}



// Private Functions
//////////////////////

void deoxrSwapchain::pCleanUp(){
	if(pImages){
		delete [] pImages;
	}
	
	if(pSwapchain){
		pSession.GetSystem().GetInstance().xrDestroySwapchain(pSwapchain);
		pSwapchain = XR_NULL_HANDLE;
	}
}

void deoxrSwapchain::pGetImages(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	uint32_t count, i;
	OXR_CHECK(instance.xrEnumerateSwapchainImages(pSwapchain, 0, &count, nullptr));
	
	if(count == 0){
		return;
	}
	
	pImages = new sImage[count];
	
	switch(pSession.GetGraphicApi()){
	case deoxrSession::egaOpenGL:{
		#ifdef OS_ANDROID
			XrSwapchainImageOpenGLESKHR *images = new XrSwapchainImageOpenGLESKHR[count];
			memset(images, 0, sizeof(XrSwapchainImageOpenGLESKHR) * count);
		#else
			XrSwapchainImageOpenGLKHR *images = new XrSwapchainImageOpenGLKHR[count];
			memset(images, 0, sizeof(XrSwapchainImageOpenGLKHR) * count);
		#endif
		
		try{
			for(i=0; i<count; i++){
				#ifdef OS_ANDROID
				images[i].type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_ES_KHR;
				#else
				images[i].type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR;
				#endif
			}
			
			OXR_CHECK(instance.xrEnumerateSwapchainImages(pSwapchain,
				count, &count, (XrSwapchainImageBaseHeader*)images));
			
			for(i=0; i<count; i++){
				pImages[i].openglImage = images[i].image;
// 				pImages[ i ].openglFbo.TakeOver( new deoxrGraphicApiOpenGL::Framebuffer(
// 					oxr.GetGraphicApiOpenGL(), images[ i ].image ) );
			}
			pImageCount = (int)count;
			delete [] images;
			
		}catch(const deException &){
			delete [] images;
		}
		}break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "invalid graphic api");
	}
}
