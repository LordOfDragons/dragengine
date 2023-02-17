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
#include "../framebuffer/devkFramebuffer.h"
#include "../pipeline/devkPipeline.h"
#include "../renderpass/devkRenderPass.h"
#include "../queue/devkQueue.h"
#include "../queue/devkCommandBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkCommandBuffer
////////////////////////////

devkCommandBuffer::devkCommandBuffer( devkCommandPool &pool ) :
pPool( pool ),
pBuffer( VK_NULL_HANDLE ),
pRecording( false ),
pBoundPipeline( nullptr ),
pActiveRenderPass( nullptr ),
pFence( VK_NULL_HANDLE ),
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
		VK_CHECK( vulkan, device.vkCreateFence( device.GetDevice(), &fenceInfo, VK_NULL_HANDLE, &pFence ) );
		
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
	DEASSERT_FALSE( pRecording )
	
	Wait( true );
	
	VkCommandBufferBeginInfo beginInfo;
	memset( &beginInfo, 0, sizeof( beginInfo ) );
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	
	VK_CHECK( pPool.GetDevice().GetInstance().GetVulkan(),
		pPool.GetDevice().vkBeginCommandBuffer( pBuffer, &beginInfo ) );
	
	pBoundPipeline = nullptr;
	pActiveRenderPass = nullptr;
	pRecording = true;
}

void devkCommandBuffer::Barrier( const devkBuffer &buffer, bool useDeviceBuffer,
VkAccessFlags sourceAccessMask, VkAccessFlags destAccessMask,
VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags destStageMask ){
	DEASSERT_TRUE( pRecording )
	
	VkBufferMemoryBarrier barrier;
	memset( &barrier, 0, sizeof( barrier ) );
	barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	barrier.buffer = useDeviceBuffer ? buffer.GetBuffer() : buffer.GetBufferHost();
	barrier.size = VK_WHOLE_SIZE;
	barrier.srcAccessMask = sourceAccessMask;
	barrier.dstAccessMask = destAccessMask;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	
	pPool.GetDevice().vkCmdPipelineBarrier( pBuffer, sourceStageMask, destStageMask,
		0, 0, VK_NULL_HANDLE, 1, &barrier, 0, VK_NULL_HANDLE );
}

void devkCommandBuffer::BarrierHostShader( const devkBuffer &buffer, VkPipelineStageFlags destStageMask ){
	Barrier( buffer, true, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_HOST_BIT, destStageMask );
}

void devkCommandBuffer::BarrierShaderTransfer( const devkBuffer &buffer, VkPipelineStageFlags srcStageMask ){
	Barrier( buffer, true, VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
		srcStageMask, VK_PIPELINE_STAGE_TRANSFER_BIT );
}

void devkCommandBuffer::BarrierTransferHost( const devkBuffer &buffer ){
	Barrier( buffer, false, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT );
}

void devkCommandBuffer::Barrier( const devkImage &image, VkAccessFlags sourceAccessMask,
VkAccessFlags destAccessMask, VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags destStageMask ){
	DEASSERT_TRUE( pRecording )
	
	VkImageMemoryBarrier barrier;
	memset( &barrier, 0, sizeof( barrier ) );
	
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image.GetImage();
	barrier.srcAccessMask = sourceAccessMask;
	barrier.dstAccessMask = destAccessMask;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	
	if( image.GetConfiguration().GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ){
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
	}else{
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = image.GetConfiguration().GetMipMapCount();
	
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = image.GetConfiguration().GetLayerCount();
	
	pPool.GetDevice().vkCmdPipelineBarrier( pBuffer, sourceStageMask, destStageMask,
		0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier );
}

void devkCommandBuffer::BarrierHostShader( const devkImage &image, VkPipelineStageFlags destStageMask ){
	Barrier( image, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_HOST_BIT, destStageMask );
}

void devkCommandBuffer::BarrierShaderTransfer( const devkImage &image, VkPipelineStageFlags srcStageMask ){
	Barrier( image, VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
		srcStageMask, VK_PIPELINE_STAGE_TRANSFER_BIT );
}

void devkCommandBuffer::BarrierTransferHost( const devkImage &image ){
	Barrier( image, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT );
}

void devkCommandBuffer::BindPipeline( const devkPipeline &pipeline ){
	DEASSERT_TRUE( pRecording )
	
	if( &pipeline == pBoundPipeline ){
		return;
	}
	
	pPool.GetDevice().vkCmdBindPipeline( pBuffer, pipeline.GetBindPoint(), pipeline.GetPipeline() );
	pBoundPipeline = &pipeline;
}

void devkCommandBuffer::BindDescriptorSet( int bindPoint, const devkDescriptorSet &descriptorSet ){
	DEASSERT_TRUE( pRecording )
	DEASSERT_NOTNULL( pBoundPipeline )
	DEASSERT_TRUE( bindPoint >= 0 )
	
	const VkDescriptorSet dset = descriptorSet.GetSet();
	pPool.GetDevice().vkCmdBindDescriptorSets( pBuffer, pBoundPipeline->GetBindPoint(),
		pBoundPipeline->GetLayout(), bindPoint, 1, &dset, 0, VK_NULL_HANDLE );
}

void devkCommandBuffer::BindVertexBuffer( int bindPoint, const devkBuffer &buffer, int offset ){
	DEASSERT_TRUE( pRecording )
	DEASSERT_NOTNULL( pActiveRenderPass )
	DEASSERT_TRUE( bindPoint >= 0 )
	DEASSERT_TRUE( offset >= 0 )
	
	const VkDeviceSize vulkanOffset = ( VkDeviceSize )offset;
	const VkBuffer vulkanBuffer = buffer.GetBuffer();
	pPool.GetDevice().vkCmdBindVertexBuffers( pBuffer, bindPoint, 1, &vulkanBuffer, &vulkanOffset );

}

void devkCommandBuffer::DispatchCompute( const decPoint3 &group ){
	DispatchCompute( group.x, group.y, group.z );
}

void devkCommandBuffer::DispatchCompute( int groupX, int groupY, int groupZ ){
	DEASSERT_TRUE( pRecording )
	DEASSERT_NOTNULL( pBoundPipeline )
	
	pPool.GetDevice().vkCmdDispatch( pBuffer, groupX, groupY, groupZ );
}

void devkCommandBuffer::BeginRenderPass( const devkRenderPass &renderPass, const devkFramebuffer &framebuffer ){
	BeginRenderPass( renderPass, framebuffer, decPoint(), framebuffer.GetConfiguration().GetSize() );
}

void devkCommandBuffer::BeginRenderPass( const devkRenderPass &renderPass, const devkFramebuffer &framebuffer,
const decPoint &position, const decPoint &size ){
	DEASSERT_TRUE( pRecording )
	DEASSERT_NULL( pActiveRenderPass )
	
	VkRenderPassBeginInfo beginInfo;
	memset( &beginInfo, 0, sizeof( beginInfo ) );
	beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.renderPass = renderPass.GetRenderPass();
	beginInfo.framebuffer = framebuffer.GetFramebuffer();
	beginInfo.renderArea.offset.x = ( int32_t )position.x;
	beginInfo.renderArea.offset.y = ( int32_t )position.y;
	beginInfo.renderArea.extent.width = ( uint32_t )size.x;
	beginInfo.renderArea.extent.height = ( uint32_t )size.y;
	beginInfo.clearValueCount = renderPass.GetConfiguration().GetAttachmentCount();
	beginInfo.pClearValues = renderPass.GetConfiguration().GetClearValues();
	
	pPool.GetDevice().vkCmdBeginRenderPass( pBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE );
	pActiveRenderPass = &renderPass;
	
	VkViewport viewport;
	viewport.x = ( float )position.x;
	viewport.y = ( float )position.y;
	viewport.width = ( float )size.x;
	viewport.height = ( float )size.y;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	
	pPool.GetDevice().vkCmdSetViewport( pBuffer, 0, 1, &viewport );
	
	VkRect2D scissor = beginInfo.renderArea;
	
	pPool.GetDevice().vkCmdSetScissor( pBuffer, 0, 1, &scissor );
}

void devkCommandBuffer::SetStencilCompareMask( uint32_t front ){
	DEASSERT_TRUE( pRecording )
	pPool.GetDevice().vkCmdSetStencilCompareMask( pBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, front );
}

void devkCommandBuffer::SetStencilCompareMask( uint32_t front, uint32_t back ){
	DEASSERT_TRUE( pRecording )
	pPool.GetDevice().vkCmdSetStencilCompareMask( pBuffer, VK_STENCIL_FACE_FRONT_BIT, front );
	pPool.GetDevice().vkCmdSetStencilCompareMask( pBuffer, VK_STENCIL_FACE_BACK_BIT, back );
}

void devkCommandBuffer::SetStencilWriteMask( uint32_t front ){
	DEASSERT_TRUE( pRecording )
	pPool.GetDevice().vkCmdSetStencilWriteMask( pBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, front );
}

void devkCommandBuffer::SetStencilWriteMask( uint32_t front, uint32_t back ){
	DEASSERT_TRUE( pRecording )
	pPool.GetDevice().vkCmdSetStencilWriteMask( pBuffer, VK_STENCIL_FACE_FRONT_BIT, front );
	pPool.GetDevice().vkCmdSetStencilWriteMask( pBuffer, VK_STENCIL_FACE_BACK_BIT, back );
}

void devkCommandBuffer::SetStencilReference( uint32_t front ){
	DEASSERT_TRUE( pRecording )
	pPool.GetDevice().vkCmdSetStencilReference( pBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, front );
}

void devkCommandBuffer::SetStencilReference( uint32_t front, uint32_t back ){
	DEASSERT_TRUE( pRecording )
	pPool.GetDevice().vkCmdSetStencilReference( pBuffer, VK_STENCIL_FACE_FRONT_BIT, front );
	pPool.GetDevice().vkCmdSetStencilReference( pBuffer, VK_STENCIL_FACE_BACK_BIT, back );
}

void devkCommandBuffer::Draw( int vertexCount, int instanceCount, int firstVertex, int firstInstance ){
	DEASSERT_TRUE( pRecording )
	DEASSERT_NOTNULL( pBoundPipeline )
	DEASSERT_NOTNULL( pActiveRenderPass )
	
	pPool.GetDevice().vkCmdDraw( pBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
}

void devkCommandBuffer::EndRenderPass(){
	DEASSERT_NOTNULL( pActiveRenderPass )
	DEASSERT_TRUE( pRecording )
	
	pPool.GetDevice().vkCmdEndRenderPass( pBuffer );
	
	pActiveRenderPass = nullptr;
}

void devkCommandBuffer::WriteBuffer( const devkBuffer &buffer ){
	VkBufferCopy copy;
	memset( &copy, 0, sizeof( copy ) );
	copy.size = buffer.GetSize();
	pPool.GetDevice().vkCmdCopyBuffer( pBuffer, buffer.GetBufferHost(), buffer.GetBuffer(), 1, &copy );
}

void devkCommandBuffer::ReadBuffer( const devkBuffer &buffer ){
	VkBufferCopy copy;
	memset( &copy, 0, sizeof( copy ) );
	copy.size = buffer.GetSize();
	pPool.GetDevice().vkCmdCopyBuffer( pBuffer, buffer.GetBuffer(), buffer.GetBufferHost(), 1, &copy );
}

void devkCommandBuffer::ReadImage( devkImage &image ){
	const devkImageConfiguration &config = image.GetConfiguration();
	VkBufferImageCopy copy;
	memset( &copy, 0, sizeof( copy ) );
	
	copy.bufferOffset = 0;
	copy.bufferRowLength = 0;
	copy.bufferImageHeight = 0;
	
	if( config.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ){
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
	}else{
		copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	copy.imageSubresource.mipLevel = 0;
	copy.imageSubresource.baseArrayLayer = 0;
	copy.imageSubresource.layerCount = config.GetLayerCount();
	
	copy.imageOffset.x = 0;
	copy.imageOffset.y = 0;
	copy.imageOffset.z = 0;
	copy.imageExtent.width = config.GetSize().x;
	copy.imageExtent.height = config.GetSize().y;
	copy.imageExtent.depth = config.GetSize().z;
	
	image.EnsureHostBuffer();
	
	pPool.GetDevice().vkCmdCopyImageToBuffer( pBuffer, image.GetImage(),
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image.GetBufferHost(), 1, &copy );
}

void devkCommandBuffer::End(){
	DEASSERT_TRUE( pRecording )
	
	if( pActiveRenderPass ){
		EndRenderPass();
	}
	
	VK_CHECK( pPool.GetDevice().GetInstance().GetVulkan(), pPool.GetDevice().vkEndCommandBuffer( pBuffer ) );
	
	pRecording = false;
	pBoundPipeline = nullptr;
}

void devkCommandBuffer::Submit( devkQueue &queue ){
	DEASSERT_FALSE( pRecording )
	
	Wait( true );
	
	devkDevice &device = pPool.GetDevice();
	VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
	
	pFenceActive = false;
	VK_CHECK( vulkan, device.vkResetFences( device.GetDevice(), 1, &pFence ) );
	
	VkSubmitInfo submitInfo;
	memset( &submitInfo, 0, sizeof( submitInfo ) );
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &pBuffer;
	
	VK_CHECK( vulkan, device.vkQueueSubmit( queue.GetQueue(), 1, &submitInfo, pFence ) );
	pFenceActive = true;
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
		#ifdef VK_CHECKCOMMANDS
		VK_CHECK( vulkan, result );
		#endif
		DETHROW( deeInvalidAction );
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
		device.vkDestroyFence( device.GetDevice(), pFence, VK_NULL_HANDLE );
	}
}
