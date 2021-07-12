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

#include "devkCommandBuffer.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../buffer/devkBuffer.h"
#include "../descriptor/devkDescriptorSetLayout.h"
#include "../descriptor/devkDescriptorSetLayoutConfiguration.h"
#include "../descriptor/devkDescriptorSet.h"
#include "../pipeline/devkPipeline.h"
#include "../queue/devkQueue.h"
#include "../queue/devkCommandBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkCommandBuffer
////////////////////////////

devkCommandBuffer::devkCommandBuffer( devkCommandPool &pool ) :
pPool( pool ),
pBuffer( nullptr ),
pRecording( false ),
pBoundPipeline( nullptr ),
pFence( nullptr ),
pFenceActive( false ),
pLLPool( this )
{
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = pool.GetDevice().GetInstance().GetVulkan() );
		devkDevice &device = pool.GetDevice();
		
		VkCommandBufferAllocateInfo allocInfo;
		memset( &allocInfo, 0, sizeof( allocInfo ) );
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = pool.GetPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		VK_CHECK( vulkan, device.vkAllocateCommandBuffers( device.GetDevice(), &allocInfo, &pBuffer ) );
		
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

devkCommandBuffer::~devkCommandBuffer(){
	pCleanUp();
}



// Management
///////////////

void devkCommandBuffer::Begin(){
	if( pRecording ){
		DETHROW_INFO( deeInvalidAction, "recording" );
	}
	
	VkCommandBufferBeginInfo beginInfo;
	memset( &beginInfo, 0, sizeof( beginInfo ) );
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	
	VK_CHECK( pPool.GetDevice().GetInstance().GetVulkan(),
		pPool.GetDevice().vkBeginCommandBuffer( pBuffer, &beginInfo ) );
	
	pBoundPipeline = nullptr;
	pRecording = true;
}

void devkCommandBuffer::Barrier( devkBuffer *buffer, VkAccessFlags sourceAccessMask,
VkAccessFlags destAccessMask, VkPipelineStageFlags sourceStageMask,
VkPipelineStageFlags destStageMask ){
	if( ! pRecording ){
		DETHROW_INFO( deeInvalidAction, "not recording" );
	}
	if( ! buffer ){
		DETHROW_INFO( deeNullPointer, "buffer" );
	}
	
	VkBufferMemoryBarrier barrier;
	memset( &barrier, 0, sizeof( barrier ) );
	barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.buffer = buffer->GetBuffer();
	barrier.size = VK_WHOLE_SIZE;
	barrier.srcAccessMask = sourceAccessMask;
	barrier.dstAccessMask = destAccessMask;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	
	pPool.GetDevice().vkCmdPipelineBarrier( pBuffer, sourceStageMask, destStageMask,
		0, 0, nullptr, 1, &barrier, 0, nullptr );
}

void devkCommandBuffer::BarrierHostShader( devkBuffer *buffer, VkPipelineStageFlags destStageMask ){
	Barrier( buffer, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_HOST_BIT, destStageMask );
}

void devkCommandBuffer::BindPipeline( devkPipeline *pipeline ){
	if( ! pRecording ){
		DETHROW_INFO( deeInvalidAction, "not recording" );
	}
	if( ! pipeline ){
		DETHROW_INFO( deeNullPointer, "pipeline" );
	}
	
	pPool.GetDevice().vkCmdBindPipeline( pBuffer, pipeline->GetBindPoint(), pipeline->GetPipeline() );
	
	pBoundPipeline = pipeline;
}

void devkCommandBuffer::BindDescriptorSet( int bindPoint, devkDescriptorSet *descriptorSet ){
	if( ! pRecording ){
		DETHROW_INFO( deeInvalidAction, "not recording" );
	}
	if( ! pBoundPipeline ){
		DETHROW_INFO( deeInvalidAction, "no pipeline bound" );
	}
	if( bindPoint < 0 ){
		DETHROW_INFO( deeInvalidParam, "bindPoint < 0" );
	}
	if( ! descriptorSet ){
		DETHROW_INFO( deeNullPointer, "descriptorSet" );
	}
	
	const VkDescriptorSet dset[ 1 ] = { descriptorSet->GetSet() };
	pPool.GetDevice().vkCmdBindDescriptorSets( pBuffer, pBoundPipeline->GetBindPoint(),
		pBoundPipeline->GetLayout(), bindPoint, 1, dset, 0, nullptr );
}

void devkCommandBuffer::DispatchCompute( const decPoint3 &group ){
	DispatchCompute( group.x, group.y, group.z );
}

void devkCommandBuffer::DispatchCompute( int groupX, int groupY, int groupZ ){
	if( ! pRecording ){
		DETHROW_INFO( deeInvalidAction, "not recording" );
	}
	if( ! pBoundPipeline ){
		DETHROW_INFO( deeInvalidAction, "no pipeline bound" );
	}
	
	pPool.GetDevice().vkCmdDispatch( pBuffer, groupX, groupY, groupZ );
}



bool devkCommandBuffer::IsFinished( bool reset ){
	if( ! pFenceActive ){
		return true;
	}
	
	devkDevice &device = pPool.GetDevice();
	VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
	
	const VkResult result = device.vkWaitForFences( device.GetDevice(), 1, &pFence, VK_TRUE, 0 );
	switch( result ){
	case VK_SUCCESS:
		if( reset ){
			pFenceActive = false;
		}
		return true;
		
	case VK_TIMEOUT:
		return false;
		
	default:
		VK_CHECK( vulkan, result );
		return false;
	}
}

void devkCommandBuffer::Wait( bool reset ){
	if( ! pFenceActive ){
		return;
	}
	
	devkDevice &device = pPool.GetDevice();
	VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
	
	VK_CHECK( vulkan, device.vkWaitForFences( device.GetDevice(), 1, &pFence, VK_TRUE, UINT64_MAX ) );
	
	if( reset ){
		VK_CHECK( vulkan, device.vkResetFences( device.GetDevice(), 1, &pFence ) );
		pFenceActive = false;
	}
}

void devkCommandBuffer::ReturnToPool(){
	pPool.ReturnCommandBuffer( this );
}



// Private Functions
//////////////////////

void devkCommandBuffer::pCleanUp(){
	devkDevice &device = pPool.GetDevice();
	
	Wait();
	
	if( pBuffer ){
		device.vkFreeCommandBuffers( device.GetDevice(), pPool.GetPool(), 1, &pBuffer );
	}
	if( pFence ){
		device.vkDestroyFence( device.GetDevice(), pFence, nullptr );
	}
}
