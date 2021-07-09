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

#include "devkBuffer.h"
#include "devkDevice.h"
#include "devkInstance.h"
#include "devkQueue.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkBuffer
// //////////////////

devkBuffer::devkBuffer( devkDevice &device, VkDeviceSize size ) :
pDevice( device ),
pSize( size ),
pBuffer( nullptr ),
pBufferHost( nullptr ),
pFence( nullptr ),
pFenceActive( false ),
pCommand( nullptr )
{
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size" );
	}
	
	try{
		pCreateBuffer( VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &pBufferHost, &pBufferHostMemory, size );
		
		pCreateBuffer( VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
			| VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &pBuffer, &pBufferMemory, size );
		
		VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
		
		VkFenceCreateInfo fenceInfo;
		memset( &fenceInfo, 0, sizeof( fenceInfo ) );
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		VK_CHECK( vulkan, device.vkCreateFence( device.GetDevice(), &fenceInfo, nullptr, &pFence ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkBuffer::~devkBuffer(){
	pCleanUp();
}



// Management
///////////////

void devkBuffer::SetData( const void *data, uint32_t size ){
	if( size != pSize ){
		DETHROW_INFO( deeInvalidParam, "size mismatch" );
	}
	SetData( data, 0, size );
}

void devkBuffer::SetData( const void *data, uint32_t offset, uint32_t size ){
	if( offset < 0 ){
		DETHROW_INFO( deeInvalidParam, "offset < 0" );
	}
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	if( size == 0 ){
		return;
	}
	if( offset + size > pSize ){
		DETHROW_INFO( deeInvalidParam, "offset + size > bufferSize" );
	}
	if( ! data ){
		DETHROW_INFO( deeNullPointer, "data" );
	}
	
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	VkDevice const device = pDevice.GetDevice();
	const bool whole = offset == 0 && size == pSize;
	void *mapped = nullptr;
	
	if( whole ){
		VK_CHECK( vulkan, pDevice.vkMapMemory( device, pBufferHostMemory, 0, VK_WHOLE_SIZE, 0, &mapped ) );
		
	}else{
		VK_CHECK( vulkan, pDevice.vkMapMemory( device, pBufferHostMemory, offset, size, 0, &mapped ) );
	}
	
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
	
	pDevice.vkUnmapMemory( device, pBufferHostMemory );
}

void devkBuffer::TransferToDevice( devkCommandPool *pool, devkQueue &queue ){
	if( ! pool ){
		DETHROW_INFO( deeNullPointer, "pool" );
	}
	
	Wait( true );
	
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	VkDevice const device = pDevice.GetDevice();
	
	pFenceActive = false;
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
	
	VkBufferCopy copyRegion;
	memset( &copyRegion, 0, sizeof( copyRegion ) );
	copyRegion.size = pSize;
	pDevice.vkCmdCopyBuffer( command, pBufferHost, pBuffer, 1, &copyRegion );
	VK_CHECK( vulkan, pDevice.vkEndCommandBuffer( command ) );
	
	VkSubmitInfo submitInfo;
	memset( &submitInfo, 0, sizeof( submitInfo ) );
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &command;
	
	VkFenceCreateInfo fenceInfo;
	memset( &fenceInfo, 0, sizeof( fenceInfo ) );
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = 0;
	
	VK_CHECK( vulkan, pDevice.vkQueueSubmit( queue.GetQueue(), 1, &submitInfo, pFence ) );
	pFenceActive = true;
}

void devkBuffer::Wait( bool reset ){
	if( ! pFenceActive ){
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



// Private Functions
//////////////////////

void devkBuffer::pCleanUp(){
	VkDevice const device = pDevice.GetDevice();
	
	Wait();
	
	if( pCommand && pCommandPool ){
		pDevice.vkFreeCommandBuffers( device, pCommandPool->GetPool(), 1, &pCommand );
	}
	if( pFence ){
		pDevice.vkDestroyFence( device, pFence, nullptr );
	}
	
	if( pBuffer ){
		pDevice.vkDestroyBuffer( device, pBuffer, nullptr );
	}
	if( pBufferMemory ){
		pDevice.vkFreeMemory( device, pBufferMemory, nullptr );
	}
	
	if( pBufferHost ){
		pDevice.vkDestroyBuffer( device, pBufferHost, nullptr );
	}
	if( pBufferHostMemory ){
		pDevice.vkFreeMemory( device, pBufferHostMemory, nullptr );
	}
}

void devkBuffer::pCreateBuffer( VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
VkBuffer* buffer, VkDeviceMemory *memory, VkDeviceSize size ){
	VK_IF_CHECK( deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan() );
	VkDevice const device = pDevice.GetDevice();
	
	// create buffer handle
	VkBufferCreateInfo bufferInfo;
	memset( &bufferInfo, 0, sizeof( bufferInfo ) );
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.usage = usage;
	bufferInfo.size = size;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VK_CHECK( vulkan, pDevice.vkCreateBuffer( device, &bufferInfo, nullptr, buffer ) );
	
	// create the memory backing up the buffer handle
	VkMemoryAllocateInfo allocInfo;
	memset( &allocInfo, 0, sizeof( allocInfo ) );
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	
	VkMemoryRequirements memoryRequirements;
	pDevice.vkGetBufferMemoryRequirements( device, *buffer, &memoryRequirements );
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = pDevice.IndexOfMemoryType( memoryProperty, memoryRequirements );
	
	VK_CHECK( vulkan, pDevice.vkAllocateMemory( device, &allocInfo, nullptr, memory ) );
	VK_CHECK( vulkan, pDevice.vkBindBufferMemory( device, *buffer, *memory, 0 ) );
}
