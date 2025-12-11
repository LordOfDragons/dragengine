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

#include "devkCommandBuffer.h"
#include "devkCommandPool.h"
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

devkCommandBuffer::devkCommandBuffer(devkCommandPool &pool) :
pPool(pool),
pBuffer(VK_NULL_HANDLE),
pRecording(false),
pBoundPipeline(nullptr),
pActiveRenderPass(nullptr),
pFence(VK_NULL_HANDLE),
pFenceActive(false),
pLLPool(this)
{
	try{
		devkDevice &device = pool.GetQueue()->GetDevice();
		VK_IF_CHECK(deSharedVulkan &vulkan = device.GetInstance().GetVulkan());
		
		VkCommandBufferAllocateInfo allocInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
		allocInfo.commandPool = pool.GetPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		VK_CHECK(vulkan, device.vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, &pBuffer));
		
		VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		VK_CHECK(vulkan, device.vkCreateFence(device.GetDevice(), &fenceInfo, VK_NULL_HANDLE, &pFence));
		
	}catch(const deException &){
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
	DEASSERT_FALSE(pRecording)
	
	Wait(true);
	
	VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	
	VK_CHECK(pPool.GetQueue()->GetDevice().GetInstance().GetVulkan(),
		pPool.GetQueue()->GetDevice().vkBeginCommandBuffer(pBuffer, &beginInfo));
	
	pBoundPipeline = nullptr;
	pActiveRenderPass = nullptr;
	pRecording = true;
}

void devkCommandBuffer::Barrier(const devkBuffer &buffer, bool useDeviceBuffer,
VkAccessFlags sourceAccessMask, VkAccessFlags destAccessMask,
VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags destStageMask){
	DEASSERT_TRUE(pRecording)
	
	VkBufferMemoryBarrier barrier{VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
	barrier.buffer = useDeviceBuffer ? buffer.GetBuffer() : buffer.GetBufferHost();
	barrier.size = VK_WHOLE_SIZE;
	barrier.srcAccessMask = sourceAccessMask;
	barrier.dstAccessMask = destAccessMask;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	
	pPool.GetQueue()->GetDevice().vkCmdPipelineBarrier(pBuffer, sourceStageMask, destStageMask,
		0, 0, VK_NULL_HANDLE, 1, &barrier, 0, VK_NULL_HANDLE);
}

void devkCommandBuffer::BarrierHostShader(const devkBuffer &buffer, VkPipelineStageFlags destStageMask){
	Barrier(buffer, true, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_HOST_BIT, destStageMask);
}

void devkCommandBuffer::BarrierShaderTransfer(const devkBuffer &buffer, VkPipelineStageFlags srcStageMask){
	Barrier(buffer, true, VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
		srcStageMask, VK_PIPELINE_STAGE_TRANSFER_BIT);
}

void devkCommandBuffer::BarrierTransferHost(const devkBuffer &buffer){
	Barrier(buffer, false, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT);
}

void devkCommandBuffer::BindPipeline(const devkPipeline &pipeline){
	DEASSERT_TRUE(pRecording)
	
	if(&pipeline == pBoundPipeline){
		return;
	}
	
	pPool.GetQueue()->GetDevice().vkCmdBindPipeline(pBuffer, pipeline.GetBindPoint(), pipeline.GetPipeline());
	pBoundPipeline = &pipeline;
}

void devkCommandBuffer::BindDescriptorSet(int bindPoint, const devkDescriptorSet &descriptorSet){
	DEASSERT_TRUE(pRecording)
	DEASSERT_NOTNULL(pBoundPipeline)
	DEASSERT_TRUE(bindPoint >= 0)
	
	const VkDescriptorSet dset = descriptorSet.GetSet();
	pPool.GetQueue()->GetDevice().vkCmdBindDescriptorSets(pBuffer, pBoundPipeline->GetBindPoint(),
		pBoundPipeline->GetLayout(), bindPoint, 1, &dset, 0, VK_NULL_HANDLE);
}

void devkCommandBuffer::BindVertexBuffer(int bindPoint, const devkBuffer &buffer, int offset){
	DEASSERT_TRUE(pRecording)
	DEASSERT_NOTNULL(pActiveRenderPass)
	DEASSERT_TRUE(bindPoint >= 0)
	DEASSERT_TRUE(offset >= 0)
	
	const VkDeviceSize vulkanOffset = (VkDeviceSize)offset;
	const VkBuffer vulkanBuffer = buffer.GetBuffer();
	pPool.GetQueue()->GetDevice().vkCmdBindVertexBuffers(pBuffer, bindPoint, 1, &vulkanBuffer, &vulkanOffset);

}

void devkCommandBuffer::DispatchCompute(const decPoint3 &group){
	DispatchCompute(group.x, group.y, group.z);
}

void devkCommandBuffer::DispatchCompute(int groupX, int groupY, int groupZ){
	DEASSERT_TRUE(pRecording)
	DEASSERT_NOTNULL(pBoundPipeline)
	
	pPool.GetQueue()->GetDevice().vkCmdDispatch(pBuffer, groupX, groupY, groupZ);
}

void devkCommandBuffer::BeginRenderPass(const devkRenderPass &renderPass, const devkFramebuffer &framebuffer){
	BeginRenderPass(renderPass, framebuffer, decPoint(), framebuffer.GetConfiguration().GetSize());
}

void devkCommandBuffer::BeginRenderPass(const devkRenderPass &renderPass, const devkFramebuffer &framebuffer,
const decPoint &position, const decPoint &size){
	DEASSERT_TRUE(pRecording)
	DEASSERT_NULL(pActiveRenderPass)
	
	VkRenderPassBeginInfo beginInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
	beginInfo.renderPass = renderPass.GetRenderPass();
	beginInfo.framebuffer = framebuffer.GetFramebuffer();
	beginInfo.renderArea.offset.x = (int32_t)position.x;
	beginInfo.renderArea.offset.y = (int32_t)position.y;
	beginInfo.renderArea.extent.width = (uint32_t)size.x;
	beginInfo.renderArea.extent.height = (uint32_t)size.y;
	beginInfo.clearValueCount = renderPass.GetConfiguration().GetAttachmentCount();
	beginInfo.pClearValues = renderPass.GetConfiguration().GetClearValues();
	
	pPool.GetQueue()->GetDevice().vkCmdBeginRenderPass(pBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	pActiveRenderPass = &renderPass;
	
	VkViewport viewport;
	viewport.x = (float)position.x;
	viewport.y = (float)position.y;
	viewport.width = (float)size.x;
	viewport.height = (float)size.y;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	
	pPool.GetQueue()->GetDevice().vkCmdSetViewport(pBuffer, 0, 1, &viewport);
	
	VkRect2D scissor = beginInfo.renderArea;
	
	pPool.GetQueue()->GetDevice().vkCmdSetScissor(pBuffer, 0, 1, &scissor);
}

void devkCommandBuffer::SetStencilCompareMask(uint32_t front){
	DEASSERT_TRUE(pRecording)
	pPool.GetQueue()->GetDevice().vkCmdSetStencilCompareMask(pBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, front);
}

void devkCommandBuffer::SetStencilCompareMask(uint32_t front, uint32_t back){
	DEASSERT_TRUE(pRecording)
	pPool.GetQueue()->GetDevice().vkCmdSetStencilCompareMask(pBuffer, VK_STENCIL_FACE_FRONT_BIT, front);
	pPool.GetQueue()->GetDevice().vkCmdSetStencilCompareMask(pBuffer, VK_STENCIL_FACE_BACK_BIT, back);
}

void devkCommandBuffer::SetStencilWriteMask(uint32_t front){
	DEASSERT_TRUE(pRecording)
	pPool.GetQueue()->GetDevice().vkCmdSetStencilWriteMask(pBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, front);
}

void devkCommandBuffer::SetStencilWriteMask(uint32_t front, uint32_t back){
	DEASSERT_TRUE(pRecording)
	pPool.GetQueue()->GetDevice().vkCmdSetStencilWriteMask(pBuffer, VK_STENCIL_FACE_FRONT_BIT, front);
	pPool.GetQueue()->GetDevice().vkCmdSetStencilWriteMask(pBuffer, VK_STENCIL_FACE_BACK_BIT, back);
}

void devkCommandBuffer::SetStencilReference(uint32_t front){
	DEASSERT_TRUE(pRecording)
	pPool.GetQueue()->GetDevice().vkCmdSetStencilReference(pBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, front);
}

void devkCommandBuffer::SetStencilReference(uint32_t front, uint32_t back){
	DEASSERT_TRUE(pRecording)
	pPool.GetQueue()->GetDevice().vkCmdSetStencilReference(pBuffer, VK_STENCIL_FACE_FRONT_BIT, front);
	pPool.GetQueue()->GetDevice().vkCmdSetStencilReference(pBuffer, VK_STENCIL_FACE_BACK_BIT, back);
}

void devkCommandBuffer::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance){
	DEASSERT_TRUE(pRecording)
	DEASSERT_NOTNULL(pBoundPipeline)
	DEASSERT_NOTNULL(pActiveRenderPass)
	
	pPool.GetQueue()->GetDevice().vkCmdDraw(pBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void devkCommandBuffer::EndRenderPass(){
	DEASSERT_NOTNULL(pActiveRenderPass)
	DEASSERT_TRUE(pRecording)
	
	pPool.GetQueue()->GetDevice().vkCmdEndRenderPass(pBuffer);
	
	pActiveRenderPass = nullptr;
}

void devkCommandBuffer::End(){
	DEASSERT_TRUE(pRecording)
	
	if(pActiveRenderPass){
		EndRenderPass();
	}
	
	VK_CHECK(pPool.GetQueue()->GetDevice().GetInstance().GetVulkan(),
		pPool.GetQueue()->GetDevice().vkEndCommandBuffer(pBuffer));
	
	pRecording = false;
	pBoundPipeline = nullptr;
}

void devkCommandBuffer::Submit(){
	if(pRecording){
		End();
	}
	
	Wait(true);
	
	devkDevice &device = pPool.GetQueue()->GetDevice();
	VK_IF_CHECK(deSharedVulkan &vulkan = device.GetInstance().GetVulkan());
	
	pFenceActive = false;
	VK_CHECK(vulkan, device.vkResetFences(device.GetDevice(), 1, &pFence));
	
	VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &pBuffer;
	
	VK_CHECK(vulkan, device.vkQueueSubmit(*pPool.GetQueue(), 1, &submitInfo, pFence));
	pFenceActive = true;
}



bool devkCommandBuffer::IsFinished(bool reset){
	if(!pFenceActive){
		return true;
	}
	
	devkDevice &device = pPool.GetQueue()->GetDevice();
	VK_IF_CHECK(deSharedVulkan &vulkan = device.GetInstance().GetVulkan());
	
	const VkResult result = device.vkWaitForFences(device.GetDevice(), 1, &pFence, VK_TRUE, 0);
	switch(result){
	case VK_SUCCESS:
		if(reset){
			pFenceActive = false;
		}
		return true;
		
	case VK_TIMEOUT:
		return false;
		
	default:
		#ifdef VK_CHECKCOMMANDS
		VK_CHECK(vulkan, result);
		#endif
		DETHROW(deeInvalidAction);
		return false;
	}
}

void devkCommandBuffer::Wait(bool reset){
	if(!pFenceActive){
		return;
	}
	
	devkDevice &device = pPool.GetQueue()->GetDevice();
	VK_IF_CHECK(deSharedVulkan &vulkan = device.GetInstance().GetVulkan());
	
	VK_CHECK(vulkan, device.vkWaitForFences(device.GetDevice(), 1, &pFence, VK_TRUE, UINT64_MAX));
	
	if(reset){
		VK_CHECK(vulkan, device.vkResetFences(device.GetDevice(), 1, &pFence));
		pFenceActive = false;
	}
}

void devkCommandBuffer::ReturnToPool(){
	pPool.ReturnCommandBuffer(this);
}



// Private Functions
//////////////////////

void devkCommandBuffer::pCleanUp(){
	devkDevice &device = pPool.GetQueue()->GetDevice();
	
	Wait(false);
	
	if(pBuffer){
		device.vkFreeCommandBuffers(device.GetDevice(), pPool.GetPool(), 1, &pBuffer);
	}
	if(pFence){
		device.vkDestroyFence(device.GetDevice(), pFence, VK_NULL_HANDLE);
	}
}
