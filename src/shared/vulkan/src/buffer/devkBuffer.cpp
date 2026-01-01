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

#include "devkBuffer.h"
#include "devkGuardUnmapBuffer.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../queue/devkQueue.h"
#include "../queue/devkCommandPool.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkBuffer
/////////////////////

devkBuffer::devkBuffer(devkDevice &device, VkDeviceSize size, VkBufferUsageFlagBits usage) :
pDevice(device),
pSize((uint32_t)size),
pUsage(usage),
pBuffer(VK_NULL_HANDLE),
pBufferHost(VK_NULL_HANDLE)
{
	DEASSERT_TRUE(size >= 0)
	
	try{
		pCreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &pBufferHost, &pBufferHostMemory, size);
		
		pCreateBuffer(usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &pBuffer, &pBufferMemory, size);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkBuffer::~devkBuffer(){
	pCleanUp();
}



// Management
///////////////

void devkBuffer::SetData(const void *data){
	SetData(data, 0, pSize);
}

void devkBuffer::SetData(const void *data, uint32_t offset, uint32_t size){
	DEASSERT_TRUE(offset >= 0)
	DEASSERT_TRUE(size >= 0)
	if(size == 0){
		return;
	}
	
	DEASSERT_TRUE(offset + size <= pSize)
	DEASSERT_NOTNULL(data)
	
	VK_IF_CHECK(deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan());
	VkDevice const device = pDevice.GetDevice();
	const bool whole = offset == 0 && size == pSize;
	void *mapped = nullptr;
	
	if(whole){
		VK_CHECK(vulkan, pDevice.vkMapMemory(device, pBufferHostMemory, 0, VK_WHOLE_SIZE, 0, &mapped));
		
	}else{
		VK_CHECK(vulkan, pDevice.vkMapMemory(device, pBufferHostMemory, offset, size, 0, &mapped));
	}
	const devkGuardUnmapBuffer guardUnmapBuffer(pDevice, pBufferHostMemory);
	
	memcpy(mapped, data, size);
	
	VkMappedMemoryRange mappedRange;
	memset(&mappedRange, 0, sizeof(mappedRange));
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = pBufferHostMemory;
	
	if(whole){
		mappedRange.offset = 0;
		mappedRange.size = VK_WHOLE_SIZE;
		
	}else{
		mappedRange.offset = offset;
		mappedRange.size = size;
	}
	
	pDevice.vkFlushMappedMemoryRanges(device, 1, &mappedRange);
}

void devkBuffer::TransferToDevice(devkCommandBuffer &commandBuffer){
	DEASSERT_TRUE(commandBuffer.GetRecording())
	
	VkBufferCopy copy{};
	copy.size = pSize;
	pDevice.vkCmdCopyBuffer(commandBuffer, pBufferHost, pBuffer, 1, &copy);
}

void devkBuffer::FetchFromDevice(devkCommandBuffer &commandBuffer){
	DEASSERT_TRUE(commandBuffer.GetRecording())
	
	VkBufferCopy copy{};
	copy.size = pSize;
	pDevice.vkCmdCopyBuffer(commandBuffer, pBuffer, pBufferHost, 1, &copy);
}

void devkBuffer::GetData(void *data){
	GetData(data, 0, pSize);
}

void devkBuffer::GetData(void *data, uint32_t offset, uint32_t size){
	DEASSERT_TRUE(offset >= 0)
	DEASSERT_TRUE(size >= 0)
	if(size == 0){
		return;
	}
	
	DEASSERT_TRUE(offset + size <= pSize)
	DEASSERT_NOTNULL(data)
	
	Wait();
	
	VK_IF_CHECK(deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan());
	const bool whole = offset == 0 && size == pSize;
	VkDevice const device = pDevice.GetDevice();
	
	void *mapped = nullptr;
	VK_CHECK(vulkan, pDevice.vkMapMemory(device, pBufferHostMemory, 0, VK_WHOLE_SIZE, 0, &mapped));
	const devkGuardUnmapBuffer guardUnmapBuffer(pDevice, pBufferHostMemory);
	
	VkMappedMemoryRange mappedRange{VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE};
	mappedRange.memory = pBufferHostMemory;
	
	if(whole){
		mappedRange.offset = 0;
		mappedRange.size = VK_WHOLE_SIZE;
		
	}else{
		mappedRange.offset = offset;
		mappedRange.size = size;
	}
	
	VK_CHECK(vulkan, pDevice.vkInvalidateMappedMemoryRanges(device, 1, &mappedRange));
	
	memcpy(data, mapped, size);
}

void devkBuffer::Wait(){
	if(pCommandBuffer){
		pCommandBuffer->Wait();
		pCommandBuffer = nullptr;
	}
}



// Private Functions
//////////////////////

devkCommandBuffer &devkBuffer::BeginCommandBuffer(devkCommandPool &pool){
	Wait();
	
	pCommandBuffer = pool.GetCommandBuffer();
	pCommandBuffer->Begin();
	return pCommandBuffer;
}

void devkBuffer::pCleanUp(){
	VkDevice const device = pDevice.GetDevice();
	
	Wait();
	
	if(pBuffer){
		pDevice.vkDestroyBuffer(device, pBuffer, VK_NULL_HANDLE);
	}
	if(pBufferMemory){
		pDevice.vkFreeMemory(device, pBufferMemory, VK_NULL_HANDLE);
	}
	
	if(pBufferHost){
		pDevice.vkDestroyBuffer(device, pBufferHost, VK_NULL_HANDLE);
	}
	if(pBufferHostMemory){
		pDevice.vkFreeMemory(device, pBufferHostMemory, VK_NULL_HANDLE);
	}
}

void devkBuffer::pCreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
VkBuffer* buffer, VkDeviceMemory *memory, VkDeviceSize size){
	VK_IF_CHECK(deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan());
	VkDevice const device = pDevice.GetDevice();
	
	// create buffer handle
	VkBufferCreateInfo bufferInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
	bufferInfo.usage = usage;
	bufferInfo.size = size;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VK_CHECK(vulkan, pDevice.vkCreateBuffer(device, &bufferInfo, VK_NULL_HANDLE, buffer));
	
	// create the memory backing up the buffer handle
	VkMemoryAllocateInfo allocInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
	
	VkMemoryRequirements memoryRequirements;
	pDevice.vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = pDevice.IndexOfMemoryType(memoryProperty, memoryRequirements);
	
	VK_CHECK(vulkan, pDevice.vkAllocateMemory(device, &allocInfo, VK_NULL_HANDLE, memory));
	VK_CHECK(vulkan, pDevice.vkBindBufferMemory(device, *buffer, *memory, 0));
}
