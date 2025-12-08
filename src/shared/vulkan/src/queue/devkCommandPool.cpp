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

#include "devkCommandPool.h"
#include "devkCommandBuffer.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkCommandPool
//////////////////////////

devkCommandPool::devkCommandPool(const devkQueue::Ref &queue) :
pQueue(queue),
pPool(VK_NULL_HANDLE)
{
	try{
		devkDevice &device = queue->GetDevice();
		VK_IF_CHECK(deSharedVulkan &vulkan = device.GetInstance().GetVulkan());
		
		VkCommandPoolCreateInfo info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
		info.queueFamilyIndex = queue->GetFamily();
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		
		VK_CHECK(vulkan, device.vkCreateCommandPool(device, &info, VK_NULL_HANDLE, &pPool));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkCommandPool::~devkCommandPool(){
	pCleanUp();
}



// Management
///////////////

devkCommandBuffer *devkCommandPool::GetCommandBuffer(){
	// return next free command buffer not require waiting if present. the linked list is
	// searched form the front. this way the oldest command buffer is checked first.
	// if this command buffer is not finished yet all command buffers after it will not
	// be finished either. this works since submitted command buffers are added to the
	// end of the list
	if(pFreeCommandBuffers.GetCount() > 0){
		devkCommandBuffer * const commandBuffer =
			(devkCommandBuffer*)pFreeCommandBuffers.GetAt(0)->GetOwner();
		if(commandBuffer->IsFinished(true)){
			commandBuffer->AddReference(); // caller holds reference
			pFreeCommandBuffers.Remove(&commandBuffer->GetLLPool());
			return commandBuffer;
		}
	}
	
	// no free command buffer not require waiting. allocate a new one
	return new devkCommandBuffer(*this);
}

void devkCommandPool::ReturnCommandBuffer(devkCommandBuffer::Ref commandBuffer){
	DEASSERT_NOTNULL(commandBuffer)
	if(&commandBuffer->GetPool() != this){
		DETHROW_INFO(deeInvalidParam, "commandBuffer pool mismatch");
	}
	
	pFreeCommandBuffers.Add(&commandBuffer->GetLLPool());
	commandBuffer->FreeReference(); // caller held reference
}



// Private Functions
//////////////////////

void devkCommandPool::pCleanUp(){
	pFreeCommandBuffers.RemoveAll();
	
	if(pPool){
		pQueue->GetDevice().vkDestroyCommandPool(pQueue->GetDevice(), pPool, VK_NULL_HANDLE);
	}
}
