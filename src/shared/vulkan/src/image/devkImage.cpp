/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <string.h>

#include "devkImage.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"
#include "../buffer/devkGuardUnmapBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkImage
///////////////////////

devkImage::devkImage( devkDevice &device, const devkImageConfiguration &configuration ) :
pDevice( device ),
pConfiguration( configuration ),
pImage( VK_NULL_HANDLE ),
pImageSize( 0 ),
pMemory( VK_NULL_HANDLE ),
pBufferHost( VK_NULL_HANDLE ),
pBufferHostMemory( VK_NULL_HANDLE ),
pFence( VK_NULL_HANDLE ),
pFenceActive( false ),
pCommand( VK_NULL_HANDLE )
{
	try{
		deSharedVulkan &vulkan = device.GetInstance().GetVulkan();
		
		VkImageCreateInfo imageInfo;
		memset( &imageInfo, 0, sizeof( imageInfo ) );
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.flags = configuration.GetFlags();
		imageInfo.imageType = configuration.GetType();
		imageInfo.format = configuration.GetFormat();
		imageInfo.extent.width = ( uint32_t )configuration.GetSize().x;
		imageInfo.extent.height = ( uint32_t )configuration.GetSize().y;
		imageInfo.extent.depth = ( uint32_t )configuration.GetSize().z;
		imageInfo.mipLevels = ( uint32_t )configuration.GetMipMapCount();
		imageInfo.arrayLayers = ( uint32_t )configuration.GetLayerCount();
		imageInfo.samples = configuration.GetSamples();
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = configuration.GetUsage();
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		
		VK_CHECK( vulkan, device.vkCreateImage( device.GetDevice(), &imageInfo, VK_NULL_HANDLE, &pImage ) );
		
		// create the memory backing up the image
		VkMemoryAllocateInfo allocInfo;
		memset( &allocInfo, 0, sizeof( allocInfo ) );
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		
		VkMemoryRequirements memoryRequirements;
		pDevice.vkGetImageMemoryRequirements( device.GetDevice(), pImage, &memoryRequirements );
		pImageSize = memoryRequirements.size;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = pDevice.IndexOfMemoryType(
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryRequirements );
		
		VK_CHECK( vulkan, pDevice.vkAllocateMemory( device.GetDevice(), &allocInfo, VK_NULL_HANDLE, &pMemory ) );
		VK_CHECK( vulkan, pDevice.vkBindImageMemory( device.GetDevice(), pImage, pMemory, 0 ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkImage::~devkImage(){
	pCleanUp();
}



// Management
///////////////

void devkImage::SetData( const void *data ){
	SetData( data, 0, pImageSize );
}

void devkImage::SetData( const void *data, uint32_t offset, uint32_t size ){
	if( offset < 0 ){
		DETHROW_INFO( deeInvalidParam, "offset < 0" );
	}
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	if( size == 0 ){
		return;
	}
	if( offset + size > pImageSize ){
		DETHROW_INFO( deeInvalidParam, "offset + size > imageSize" );
	}
	if( ! data ){
		DETHROW_INFO( deeNullPointer, "data" );
	}
	
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	VkDevice const device = pDevice.GetDevice();
	const bool whole = offset == 0 && size == pImageSize;
	void *mapped = nullptr;
	
	if( ! pBufferHostMemory ){
		pCreateBuffer( VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &pBufferHost, &pBufferHostMemory, size );
	}
	
	if( whole ){
		VK_CHECK( vulkan, pDevice.vkMapMemory( device, pBufferHostMemory, 0, VK_WHOLE_SIZE, 0, &mapped ) );
		
	}else{
		VK_CHECK( vulkan, pDevice.vkMapMemory( device, pBufferHostMemory, offset, size, 0, &mapped ) );
	}
	const devkGuardUnmapBuffer guardUnmapBuffer( pDevice, pBufferHostMemory );
	
	memcpy( mapped, data, size );
	
	VkMappedMemoryRange mappedRange;
	memset( &mappedRange, 0, sizeof( mappedRange ) );
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = pBufferHostMemory;
	
	if( whole ){
		mappedRange.offset = 0;
		mappedRange.size = VK_WHOLE_SIZE;
		
	}else{
		mappedRange.offset = offset;
		mappedRange.size = size;
	}
	
	pDevice.vkFlushMappedMemoryRanges( device, 1, &mappedRange );
}

void devkImage::TransferToDevice( devkCommandPool *pool, devkQueue &queue ){
	if( ! pool ){
		DETHROW_INFO( deeNullPointer, "pool" );
	}
	if( ! pBufferHost || ! pBufferHostMemory ){
		DETHROW_INFO( deeNullPointer, "bufferHost" );
	}
	
	Wait( true );
	
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	VkDevice const device = pDevice.GetDevice();
	
	pFenceActive = false;
	if( ! pFence ){
		pCreateFence();
	}
	VK_CHECK( vulkan, pDevice.vkResetFences( device, 1, &pFence ) );
	
	VkCommandBufferAllocateInfo allocInfo;
	memset( &allocInfo, 0, sizeof( allocInfo ) );
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = pool->GetPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;
	
	VkCommandBuffer command;
	VK_CHECK( vulkan, pDevice.vkAllocateCommandBuffers( device, &allocInfo, &command ) );
	pCommandPool = pool;
	
	VkCommandBufferBeginInfo commandInfo;
	memset( &commandInfo, 0, sizeof( commandInfo ) );
	commandInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	VK_CHECK( vulkan, pDevice.vkBeginCommandBuffer( command, &commandInfo ) );
	
	VkBufferImageCopy copyRegion;
	memset( &copyRegion, 0, sizeof( copyRegion ) );
	
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;
	
	if( pConfiguration.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ){
		copyRegion.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
		copyRegion.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
	}else{
		copyRegion.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = pConfiguration.GetLayerCount();
	
	copyRegion.imageOffset.x = 0;
	copyRegion.imageOffset.y = 0;
	copyRegion.imageOffset.z = 0;
	copyRegion.imageExtent.width = pConfiguration.GetSize().x;
	copyRegion.imageExtent.height = pConfiguration.GetSize().y;
	copyRegion.imageExtent.depth = pConfiguration.GetSize().z;
	
	pDevice.vkCmdCopyBufferToImage( command, pBufferHost, pImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion );
	VK_CHECK( vulkan, pDevice.vkEndCommandBuffer( command ) );
	
	VkSubmitInfo submitInfo;
	memset( &submitInfo, 0, sizeof( submitInfo ) );
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &command;
	
	VK_CHECK( vulkan, pDevice.vkQueueSubmit( queue.GetQueue(), 1, &submitInfo, pFence ) );
	pFenceActive = true;
}

void devkImage::GetData( void *data ){
	GetData( data, 0, pImageSize );
}

void devkImage::GetData( void *data, uint32_t offset, uint32_t size ){
	if( offset < 0 ){
		DETHROW_INFO( deeInvalidParam, "offset < 0" );
	}
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	if( size == 0 ){
		return;
	}
	if( offset + size > pImageSize ){
		DETHROW_INFO( deeInvalidParam, "offset + size > imageSize" );
	}
	if( ! data ){
		DETHROW_INFO( deeNullPointer, "data" );
	}
	if( ! pBufferHost || ! pBufferHostMemory ){
		DETHROW_INFO( deeNullPointer, "bufferHost" );
	}
	
	Wait( true );
	
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	const bool whole = offset == 0 && size == pImageSize;
	VkDevice const device = pDevice.GetDevice();
	
	void *mapped = nullptr;
	VK_CHECK( vulkan, pDevice.vkMapMemory( device, pBufferHostMemory, 0, VK_WHOLE_SIZE, 0, &mapped ) );
	const devkGuardUnmapBuffer guardUnmapBuffer( pDevice, pBufferHostMemory );
	
	VkMappedMemoryRange mappedRange;
	memset( &mappedRange, 0, sizeof( mappedRange ) );
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = pBufferHostMemory;
	
	if( whole ){
		mappedRange.offset = 0;
		mappedRange.size = VK_WHOLE_SIZE;
		
	}else{
		mappedRange.offset = offset;
		mappedRange.size = size;
	}
	
	VK_CHECK( vulkan, pDevice.vkInvalidateMappedMemoryRanges( device, 1, &mappedRange ) );
	
	memcpy( data, mapped, size );
}

void devkImage::Wait( bool reset ){
	if( ! pFenceActive || ! pFence ){
		return;
	}
	
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	VkDevice const device = pDevice.GetDevice();
	
	VK_CHECK( vulkan, pDevice.vkWaitForFences( device, 1, &pFence, VK_TRUE, UINT64_MAX ) );
	
	if( reset ){
		VK_CHECK( vulkan, pDevice.vkResetFences( device, 1, &pFence ) );
		pFenceActive = false;
	}
}

void devkImage::DropTransferResources(){
	Wait();
	
	VkDevice const device = pDevice.GetDevice();
	
	if( pCommand && pCommandPool ){
		pDevice.vkFreeCommandBuffers( device, pCommandPool->GetPool(), 1, &pCommand );
	}
	pCommand = VK_NULL_HANDLE;
	pCommandPool = nullptr;
	
	if( pFence ){
		pDevice.vkDestroyFence( device, pFence, VK_NULL_HANDLE );
		pFence = VK_NULL_HANDLE;
	}
	
	if( pBufferHost ){
		pDevice.vkDestroyBuffer( device, pBufferHost, VK_NULL_HANDLE );
		pBufferHost = VK_NULL_HANDLE;
	}
	if( pBufferHostMemory ){
		pDevice.vkFreeMemory( device, pBufferHostMemory, VK_NULL_HANDLE );
		pBufferHostMemory = VK_NULL_HANDLE;
	}
}

void devkImage::EnsureHostBuffer(){
	Wait();
	
	if( ! pBufferHostMemory ){
		pCreateBuffer( VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &pBufferHost, &pBufferHostMemory, pImageSize );
	}
}



// Private Functions
//////////////////////

void devkImage::pCleanUp(){
	VkDevice const device = pDevice.GetDevice();
	
	Wait();
	
	if( pCommand && pCommandPool ){
		pDevice.vkFreeCommandBuffers( device, pCommandPool->GetPool(), 1, &pCommand );
	}
	if( pFence ){
		pDevice.vkDestroyFence( device, pFence, VK_NULL_HANDLE );
	}
	
	if( pImage ){
		pDevice.vkDestroyImage( device, pImage, VK_NULL_HANDLE );
	}
	if( pMemory ){
		pDevice.vkFreeMemory( device, pMemory, VK_NULL_HANDLE );
	}
	
	if( pBufferHost ){
		pDevice.vkDestroyBuffer( device, pBufferHost, VK_NULL_HANDLE );
	}
	if( pBufferHostMemory ){
		pDevice.vkFreeMemory( device, pBufferHostMemory, VK_NULL_HANDLE );
	}
}

void devkImage::pCreateBuffer( VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size ){
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	VkDevice const device = pDevice.GetDevice();
	
	// create buffer handle
	VkBufferCreateInfo bufferInfo;
	memset( &bufferInfo, 0, sizeof( bufferInfo ) );
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.usage = usage;
	bufferInfo.size = size;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VK_CHECK( vulkan, pDevice.vkCreateBuffer( device, &bufferInfo, VK_NULL_HANDLE, buffer ) );
	
	// create the memory backing up the buffer handle
	VkMemoryAllocateInfo allocInfo;
	memset( &allocInfo, 0, sizeof( allocInfo ) );
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	
	VkMemoryRequirements memoryRequirements;
	pDevice.vkGetBufferMemoryRequirements( device, *buffer, &memoryRequirements );
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = pDevice.IndexOfMemoryType( memoryProperty, memoryRequirements );
	
	VK_CHECK( vulkan, pDevice.vkAllocateMemory( device, &allocInfo, VK_NULL_HANDLE, memory ) );
	VK_CHECK( vulkan, pDevice.vkBindBufferMemory( device, *buffer, *memory, 0 ) );
}

void devkImage::pCreateFence(){
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	
	VkFenceCreateInfo fenceInfo;
	memset( &fenceInfo, 0, sizeof( fenceInfo ) );
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	VK_CHECK( vulkan, pDevice.vkCreateFence( pDevice.GetDevice(), &fenceInfo, VK_NULL_HANDLE, &pFence ) );
}
