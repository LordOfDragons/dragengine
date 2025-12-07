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

#ifndef _DEVKCOMMANDBUFFER_H_
#define _DEVKCOMMANDBUFFER_H_

#include "../devkBasics.h"
#include "../devkTPoolReference.h"
#include "../queue/devkCommandPool.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectLinkedList.h>
#include <dragengine/common/math/decMath.h>

class devkDevice;
class devkBuffer;
class devkPipeline;
class devkDescriptorSet;
class devkRenderPass;
class devkFramebuffer;
class devkImage;


/**
 * Vulkan command buffer.
 */
class devkCommandBuffer : public deObject{
public:
	/** Reference. */
	typedef devkTPoolReference<devkCommandBuffer> Ref;
	
	
	
private:
	devkCommandPool &pPool;
	
	VkCommandBuffer pBuffer;
	bool pRecording;
	const devkPipeline *pBoundPipeline;
	const devkRenderPass *pActiveRenderPass;
	
	VkFence pFence;
	bool pFenceActive;
	
	decObjectLinkedList::cListEntry pLLPool;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create command buffer. */
	devkCommandBuffer(devkCommandPool &pool);
	
protected:
	/** Clean up command buffer. */
	~devkCommandBuffer() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Command pool. */
	inline devkCommandPool &GetPool() const{ return pPool; }
	
	/** Command buffer. */
	inline VkCommandBuffer GetBuffer() const{ return pBuffer; }
	inline operator VkCommandBuffer() const{ return pBuffer; }
	
	
	/** Begin recording command buffer. */
	void Begin();
	
	/** Command buffer is recording. */
	inline bool GetRecording() const{ return pRecording; }
	
	/** Add buffer memory barrier. */
	void Barrier(const devkBuffer &buffer, bool useDeviceBuffer, VkAccessFlags sourceAccessMask,
		VkAccessFlags destAccessMask, VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags destStageMask);
	
	/** Add buffer memory barrier between host writing and shader reading. */
	void BarrierHostShader(const devkBuffer &buffer, VkPipelineStageFlags destStageMask);
	
	/** Add buffer memory barrier between shader writing and transfer reading. */
	void BarrierShaderTransfer(const devkBuffer &buffer, VkPipelineStageFlags srcStageMask);
	
	/** Add buffer memory barrier between transfer writing and host reading. */
	void BarrierTransferHost(const devkBuffer &buffer);
	
	/** Bind pipeline. */
	void BindPipeline(const devkPipeline &pipeline);
	
	/** Bind descriptor set. */
	void BindDescriptorSet(int bindPoint, const devkDescriptorSet &descriptorSet);
	
	/** Bind vertex buffers. */
	void BindVertexBuffer(int bindPoint, const devkBuffer &buffer, int offset = 0);
	
	/** Dispatch compute shader. */
	void DispatchCompute(const decPoint3 &group);
	void DispatchCompute(int groupX, int groupY, int groupZ);
	
	/** Begin render pass. */
	void BeginRenderPass(const devkRenderPass &renderPass, const devkFramebuffer &framebuffer);
	
	void BeginRenderPass(const devkRenderPass &renderPass, const devkFramebuffer &framebuffer,
		const decPoint &position, const decPoint &size);
	
	/** Set stencil compare mask. */
	void SetStencilCompareMask(uint32_t front);
	void SetStencilCompareMask(uint32_t front, uint32_t back);
	
	/** Set stencil write mask. */
	void SetStencilWriteMask(uint32_t front);
	void SetStencilWriteMask(uint32_t front, uint32_t back);
	
	/** Set stencil reference. */
	void SetStencilReference(uint32_t front);
	void SetStencilReference(uint32_t front, uint32_t back);
	
	/** Draw. */
	void Draw(int vertexCount, int instanceCount, int firstVertex = 0, int firstInstance = 0);
	
	/** End render pass. */
	void EndRenderPass();
	
	/** End recording command buffer. */
	void End();
	
	/**
	 * Submit to queue.
	 * 
	 * Calls End() before submitting if command buffer is in recording state.
	 */
	void Submit();
	
	
	
	/**
	 * Check if fence is signaled.
	 * \param[in] reset If true reset fence and set it inactive after wait finished.
	 */
	bool IsFinished(bool reset = false);
	
	/**
	 * If fence is active wait for fence to be signaled.
	 * \param[in] reset If true reset fence and set it inactive after wait finished.
	 */
	void Wait(bool reset = true);
	
	/** Return to pool. */
	void ReturnToPool();
	
	/** Command pool linked list. */
	inline decObjectLinkedList::cListEntry &GetLLPool(){ return pLLPool; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

