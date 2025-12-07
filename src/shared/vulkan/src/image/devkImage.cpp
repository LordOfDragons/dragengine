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

#include "devkImage.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"
#include "../buffer/devkGuardUnmapBuffer.h"
#include "../queue/devkCommandBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkImage
////////////////////

devkImage::devkImage(devkDevice &device, const devkImageConfiguration &configuration) :
pDevice(device),
pConfiguration(configuration),
pImage(VK_NULL_HANDLE),
pImageSize(0),
pMemory(VK_NULL_HANDLE),
pBufferHost(VK_NULL_HANDLE),
pBufferHostMemory(VK_NULL_HANDLE)
{
	try{
		VK_IF_CHECK(deSharedVulkan &vulkan = device.GetInstance().GetVulkan();)
		
		VkImageCreateInfo imageInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		imageInfo.flags = configuration.GetFlags();
		imageInfo.imageType = configuration.GetType();
		imageInfo.format = configuration.GetFormat();
		imageInfo.extent.width = (uint32_t)configuration.GetSize().x;
		imageInfo.extent.height = (uint32_t)configuration.GetSize().y;
		imageInfo.extent.depth = (uint32_t)configuration.GetSize().z;
		imageInfo.mipLevels = (uint32_t)configuration.GetMipMapCount();
		imageInfo.arrayLayers = (uint32_t)configuration.GetLayerCount();
		imageInfo.samples = configuration.GetSamples();
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = configuration.GetUsage();
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		
		VK_CHECK(vulkan, device.vkCreateImage(device.GetDevice(), &imageInfo, VK_NULL_HANDLE, &pImage));
		
		// create the memory backing up the image
		VkMemoryAllocateInfo allocInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
		
		VkMemoryRequirements memoryRequirements;
		pDevice.vkGetImageMemoryRequirements(device.GetDevice(), pImage, &memoryRequirements);
		pImageSize = (uint32_t)memoryRequirements.size;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = pDevice.IndexOfMemoryType(
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryRequirements);
		
		VK_CHECK(vulkan, pDevice.vkAllocateMemory(device.GetDevice(), &allocInfo, VK_NULL_HANDLE, &pMemory));
		VK_CHECK(vulkan, pDevice.vkBindImageMemory(device.GetDevice(), pImage, pMemory, 0));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkImage::~devkImage(){
	pCleanUp();
}



// Management
///////////////

void devkImage::SetData(const void *data){
	SetData(data, 0, pImageSize);
}

void devkImage::SetData(const void *data, uint32_t offset, uint32_t size){
	if(offset < 0){
		DETHROW_INFO(deeInvalidParam, "offset < 0");
	}
	if(size < 0){
		DETHROW_INFO(deeInvalidParam, "size < 0");
	}
	if(size == 0){
		return;
	}
	if(offset + size > pImageSize){
		DETHROW_INFO(deeInvalidParam, "offset + size > imageSize");
	}
	if(!data){
		DETHROW_INFO(deeNullPointer, "data");
	}
	
	VK_IF_CHECK(deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan());
	VkDevice const device = pDevice.GetDevice();
	const bool whole = offset == 0 && size == pImageSize;
	void *mapped = nullptr;
	
	if(!pBufferHostMemory){
		pCreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &pBufferHost, &pBufferHostMemory, size);
	}
	
	if(whole){
		VK_CHECK(vulkan, pDevice.vkMapMemory(device, pBufferHostMemory, 0, VK_WHOLE_SIZE, 0, &mapped));
		
	}else{
		VK_CHECK(vulkan, pDevice.vkMapMemory(device, pBufferHostMemory, offset, size, 0, &mapped));
	}
	const devkGuardUnmapBuffer guardUnmapBuffer(pDevice, pBufferHostMemory);
	
	memcpy(mapped, data, size);
	
	VkMappedMemoryRange mappedRange{VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE};
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

void devkImage::TransferToDevice(devkCommandBuffer &commandBuffer){
	DEASSERT_NOTNULL(pBufferHost)
	DEASSERT_NOTNULL(pBufferHostMemory)
	DEASSERT_TRUE(commandBuffer.GetRecording())
	
	VkBufferImageCopy copy{};
	
	if(pConfiguration.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT){
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
	}else{
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	copy.imageSubresource.layerCount = pConfiguration.GetLayerCount();
	
	copy.imageExtent.width = pConfiguration.GetSize().x;
	copy.imageExtent.height = pConfiguration.GetSize().y;
	copy.imageExtent.depth = pConfiguration.GetSize().z;
	
	EnsureHostBuffer();
	
	pDevice.vkCmdCopyBufferToImage(commandBuffer.GetBuffer(), pBufferHost, pImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
}

void devkImage::FetchFromDevice(devkCommandBuffer &commandBuffer){
	DEASSERT_TRUE(commandBuffer.GetRecording())
	
	VkBufferImageCopy copy{};
	
	if(pConfiguration.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT){
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
	}else{
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	copy.imageSubresource.layerCount = pConfiguration.GetLayerCount();
	
	copy.imageExtent.width = pConfiguration.GetSize().x;
	copy.imageExtent.height = pConfiguration.GetSize().y;
	copy.imageExtent.depth = pConfiguration.GetSize().z;
	
	EnsureHostBuffer();
	
	pDevice.vkCmdCopyImageToBuffer(commandBuffer.GetBuffer(), pImage,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, pBufferHost, 1, &copy);
}

void devkImage::GenerateMipMaps(devkCommandBuffer &commandBuffer){
	BarrierLayoutTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	
	VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	barrier.image = pImage;
	barrier.dstQueueFamilyIndex = barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;
	
	VkImageBlit blit{};
	blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blit.srcSubresource.layerCount = 1;
	blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blit.dstSubresource.layerCount = 1;
	
	const int levelCount = pConfiguration.GetMipMapCount();
	int mipWidth = pConfiguration.GetSize().x;
	int mipHeight = pConfiguration.GetSize().y;
	int i;
	
	for(i=1; i<levelCount; i++){
		// transition layout
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		
		pDevice.vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);
		
		// blit
		blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
		blit.srcSubresource.mipLevel = i - 1;
		blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
		blit.dstSubresource.mipLevel = i;
		
		pDevice.vkCmdBlitImage(commandBuffer, pImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			pImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);
		
		// transition layout
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		
		pDevice.vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);
		
		// next round
		if(mipWidth > 1){
			mipWidth /= 2;
		}
		if(mipHeight > 1){
			mipHeight /= 2;
		}
	}
	
	// final barrier
	barrier.subresourceRange.baseMipLevel = levelCount - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	
	pDevice.vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);
}

void devkImage::GetData(void *data){
	GetData(data, 0, pImageSize);
}

void devkImage::GetData(void *data, uint32_t offset, uint32_t size){
	if(offset < 0){
		DETHROW_INFO(deeInvalidParam, "offset < 0");
	}
	if(size < 0){
		DETHROW_INFO(deeInvalidParam, "size < 0");
	}
	if(size == 0){
		return;
	}
	if(offset + size > pImageSize){
		DETHROW_INFO(deeInvalidParam, "offset + size > imageSize");
	}
	if(!data){
		DETHROW_INFO(deeNullPointer, "data");
	}
	if(!pBufferHost || !pBufferHostMemory){
		DETHROW_INFO(deeNullPointer, "bufferHost");
	}
	
	Wait();
	
	VK_IF_CHECK(deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan());
	const bool whole = offset == 0 && size == pImageSize;
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

void devkImage::Wait(){
	if(pCommandBuffer){
		pCommandBuffer->Wait();
		pCommandBuffer = nullptr;
	}
}

devkCommandBuffer & devkImage::BeginCommandBuffer(devkCommandPool &pool){
	Wait();
	
	pCommandBuffer = pool.GetCommandBuffer();
	pCommandBuffer->Begin();
	return pCommandBuffer;
}

void devkImage::DropTransferResources(){
	Wait();
	
	VkDevice const device = pDevice.GetDevice();
	
	if(pBufferHost){
		pDevice.vkDestroyBuffer(device, pBufferHost, VK_NULL_HANDLE);
		pBufferHost = VK_NULL_HANDLE;
	}
	if(pBufferHostMemory){
		pDevice.vkFreeMemory(device, pBufferHostMemory, VK_NULL_HANDLE);
		pBufferHostMemory = VK_NULL_HANDLE;
	}
}

void devkImage::EnsureHostBuffer(){
	Wait();
	
	if(!pBufferHostMemory){
		pCreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &pBufferHost, &pBufferHostMemory, pImageSize);
	}
}

void devkImage::BarrierLayoutTransition(devkCommandBuffer &commandBuffer, VkImageLayout newLayout){
	BarrierLayoutTransition(commandBuffer, VK_IMAGE_LAYOUT_UNDEFINED, newLayout);
}

void devkImage::BarrierLayoutTransition(devkCommandBuffer &commandBuffer,
VkImageLayout oldLayout, VkImageLayout newLayout){
	DEASSERT_TRUE(commandBuffer.GetRecording())
	
	VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	barrier.image = pImage;
	barrier.dstQueueFamilyIndex = barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.subresourceRange.levelCount = pConfiguration.GetMipMapCount();
	barrier.subresourceRange.layerCount = pConfiguration.GetLayerCount();
	
	if(pConfiguration.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT){
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
	}else{
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	VkPipelineStageFlags sourceStage, destinationStage;
	
	if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		
	}else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	&& newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		
	}else{
		DETHROW_INFO(deeInvalidParam, "Unsupported layout transition");
	}
	
	pDevice.vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage,
		0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);
}

void devkImage::Barrier(devkCommandBuffer &commandBuffer, VkAccessFlags sourceAccessMask,
VkAccessFlags destAccessMask, VkPipelineStageFlags sourceStageMask,
VkPipelineStageFlags destStageMask){
	DEASSERT_TRUE(commandBuffer.GetRecording())
	
	VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	barrier.image = pImage;
	barrier.srcAccessMask = sourceAccessMask;
	barrier.dstAccessMask = destAccessMask;
	barrier.dstQueueFamilyIndex = barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.newLayout = barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier.subresourceRange.levelCount = pConfiguration.GetMipMapCount();
	barrier.subresourceRange.layerCount = pConfiguration.GetLayerCount();
	
	if(pConfiguration.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT){
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
	}else{
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	pDevice.vkCmdPipelineBarrier(commandBuffer, sourceStageMask, destStageMask,
		0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);
}

void devkImage::BarrierHostShader(devkCommandBuffer &commandBuffer,
VkPipelineStageFlags destStageMask){
	Barrier(commandBuffer, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_HOST_BIT, destStageMask);
}

void devkImage::BarrierShaderTransfer(devkCommandBuffer &commandBuffer,
VkPipelineStageFlags srcStageMask){
	Barrier(commandBuffer, VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
		srcStageMask, VK_PIPELINE_STAGE_TRANSFER_BIT);
}

void devkImage::BarrierTransferHost(devkCommandBuffer &commandBuffer){
	Barrier(commandBuffer, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT);
}


// Private Functions
//////////////////////

void devkImage::pCleanUp(){
	VkDevice const device = pDevice.GetDevice();
	
	Wait();
	
	if(pImage){
		pDevice.vkDestroyImage(device, pImage, VK_NULL_HANDLE);
	}
	if(pMemory){
		pDevice.vkFreeMemory(device, pMemory, VK_NULL_HANDLE);
	}
	
	if(pBufferHost){
		pDevice.vkDestroyBuffer(device, pBufferHost, VK_NULL_HANDLE);
	}
	if(pBufferHostMemory){
		pDevice.vkFreeMemory(device, pBufferHostMemory, VK_NULL_HANDLE);
	}
}

void devkImage::pCreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size){
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
