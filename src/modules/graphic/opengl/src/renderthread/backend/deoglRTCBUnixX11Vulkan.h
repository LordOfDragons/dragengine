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

#ifndef _DEOGLRTCBUNIXVULKAN_H_
#define _DEOGLRTCBUNIXVULKAN_H_

#include <dragengine/dragengine_configuration.h>

#if defined(OS_UNIX_X11) && defined(BACKEND_VULKAN)

#include "deoglRTCBUnixX11.h"
#include <deSharedVulkan.h>
#include <devkDevice.h>
#include <queue/devkCommandPool.h>
#include <queue/devkQueue.h>


/**
 * Render thread context backend for Unix X11 with Vulkan.
 * 
 * Vulkan does not require per-window context activation like OpenGL.
 */
class deoglRTCBUnixX11Vulkan : public deoglRTCBUnixX11{
protected:
	deSharedVulkan::Ref pVulkan;
	devkDevice::Ref pDevice;
	devkQueue *pQueueGraphic;
	devkQueue *pQueueCompute;
	devkQueue *pQueueTransfer;
	devkCommandPool::Ref pCommandPoolGraphic;
	devkCommandPool::Ref pCommandPoolCompute;
	devkCommandPool::Ref pCommandPoolTransfer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBUnixX11Vulkan(deoglRTContext &context);
	
	/** Clean up backend. */
	~deoglRTCBUnixX11Vulkan() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Initialize phase 2 (render thread). */
	void InitPhase2() override;
	
	/** Clean up all resources. */
	void CleanUp() override;
	
	
	/** Vulkan. */
	inline deSharedVulkan &GetVulkan() const{ return *pVulkan; }
	
	/** Vulkan device. */
	inline devkDevice &GetDevice() const{ return *pDevice; }
	
	/** Queues. */
	inline devkQueue &GetQueueGraphic() const{ return *pQueueGraphic; }
	inline devkQueue &GetQueueCompute() const{ return *pQueueCompute; }
	inline devkQueue &GetQueueTransfer() const{ return *pQueueTransfer; }
	
	/** Command pools. */
	inline devkCommandPool &GetCommandPoolGraphic() const{ return pCommandPoolGraphic; }
	inline devkCommandPool &GetCommandPoolCompute() const{ return pCommandPoolCompute; }
	inline devkCommandPool &GetCommandPoolTransfer() const{ return pCommandPoolTransfer; }
	/*@}*/
	
	
protected:
	void pChooseVisual() override;
	void pFreeContext();
	void pCreateContext();
};

#endif // OS_UNIX_X11 && BACKEND_VULKAN
#endif // _DEOGLRTCBUNIXVULKAN_H_
