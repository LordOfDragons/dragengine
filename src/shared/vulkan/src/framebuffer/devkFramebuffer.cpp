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

#include "devkFramebuffer.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"
#include "../renderpass/devkRenderPass.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkFramebuffer
//////////////////////////

devkFramebuffer::devkFramebuffer(devkRenderPass *renderPass, const devkFramebufferConfiguration &configuration) :
pRenderPass(renderPass),
pConfiguration(configuration),
pFramebuffer(VK_NULL_HANDLE)
{
	if(! renderPass){
		DETHROW_INFO(deeNullPointer, "renderPass");
	}
	
	const int attachmentCount = configuration.GetAttachmentCount();
	int i;
	
	VkImageView *attachments = nullptr;
	
	try{
		devkDevice &device = renderPass->GetDevice();
		VK_IF_CHECK(deSharedVulkan &vulkan = device.GetInstance().GetVulkan();)
		
		VkFramebufferCreateInfo framebufferInfo{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
		framebufferInfo.renderPass = renderPass->GetRenderPass();
		framebufferInfo.width = (uint32_t)configuration.GetSize().x;
		framebufferInfo.height = (uint32_t)configuration.GetSize().y;
		framebufferInfo.layers = (uint32_t)configuration.GetLayerCount();
		
		if(attachmentCount > 0){
			attachments  = new VkImageView[attachmentCount];
			for(i=0; i<attachmentCount; i++){
				attachments[i] = configuration.GetAttachmentAt(i)->GetImageView();
			}
			
			framebufferInfo.attachmentCount = attachmentCount;
			framebufferInfo.pAttachments = attachments;
		}
		
		VK_CHECK(vulkan, device.vkCreateFramebuffer(device.GetDevice(),
			&framebufferInfo, VK_NULL_HANDLE, &pFramebuffer));
		
		if(attachments){
			delete [] attachments;
		}
		
	}catch(const deException &){
		if(attachments){
			delete [] attachments;
		}
		pCleanUp();
		throw;
	}
}

devkFramebuffer::~devkFramebuffer(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void devkFramebuffer::pCleanUp(){
	if(pFramebuffer){
		pRenderPass->GetDevice().vkDestroyFramebuffer(
			pRenderPass->GetDevice().GetDevice(), pFramebuffer, VK_NULL_HANDLE);
	}
}
