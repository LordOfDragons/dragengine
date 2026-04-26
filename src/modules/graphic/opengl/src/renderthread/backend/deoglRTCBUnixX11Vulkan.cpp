/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <dragengine/dragengine_configuration.h>

#if defined(OS_UNIX_X11) && defined(BACKEND_VULKAN)

#include "deoglRTCBUnixX11Vulkan.h"
#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>


// Class deoglRTCBUnixX11Vulkan
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBUnixX11Vulkan::deoglRTCBUnixX11Vulkan(deoglRTContext &context) :
deoglRTCBUnixX11(context),
pQueueGraphic(nullptr),
pQueueCompute(nullptr),
pQueueTransfer(nullptr){
}

deoglRTCBUnixX11Vulkan::~deoglRTCBUnixX11Vulkan() = default;


// Management
///////////////

void deoglRTCBUnixX11Vulkan::InitPhase2(){
	deoglRTCBUnixX11::InitPhase2();
	pCreateContext();
}

void deoglRTCBUnixX11Vulkan::CleanUp(){
	AssignOSWindow(0);
	pFreeContext();
	deoglRTCBUnixX11::CleanUp();
}


// Protected Functions
////////////////////////

void deoglRTCBUnixX11Vulkan::pChooseVisual(){
	// For Vulkan we choose a default visual. The exact attributes depend on the
	// Vulkan swap chain requirements. Using a simple 32-bit visual for now.
	pVisInfo = nullptr;
	
	int count = 0;
	XVisualInfo vinfo{};
	vinfo.screen = pScreen;
	XVisualInfo *list = XGetVisualInfo(pDisplay, VisualScreenMask, &vinfo, &count);
	if(list && count > 0){
		pVisInfo = (XVisualInfo*)malloc(sizeof(XVisualInfo));
		if(pVisInfo){
			*pVisInfo = list[0];
		}
		XFree(list);
	}
	
	if(!pVisInfo){
		DETHROW(deeInvalidAction);
	}
}

void deoglRTCBUnixX11Vulkan::pFreeContext(){
	pCommandPoolTransfer = nullptr;
	pCommandPoolCompute = nullptr;
	pCommandPoolGraphic = nullptr;
	pDevice = nullptr;
	pVulkan = nullptr;
}

void deoglRTCBUnixX11Vulkan::pCreateContext(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	logger.LogInfo("Creating Vulkan device");
	pVulkan = deSharedVulkan::Ref::New(pRTContext.GetRenderThread().GetOgl());
	pDevice = pVulkan->GetInstance().CreateDeviceHeadlessGraphic(0);
	
	pQueueGraphic = &pDevice->GetGraphicQueue();
	pQueueCompute = &pDevice->GetComputeQueue();
	pQueueTransfer = &pDevice->GetTransferQueue();
	
	pCommandPoolGraphic = pQueueGraphic->CreateCommandPool();
	pCommandPoolCompute = pQueueCompute->CreateCommandPool();
	pCommandPoolTransfer = pQueueTransfer->CreateCommandPool();
}

#endif // OS_UNIX_X11 && BACKEND_VULKAN
